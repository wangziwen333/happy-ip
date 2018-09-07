#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>

/*
* https://github.com/progschj/ThreadPool
*/

namespace klicen
{
	namespace utils
	{
		class ThreadPool {
		public:
			ThreadPool(const size_t);
			template<class F, class... Args>
			auto enqueue(F&& f, Args&&... args)->std::future < typename std::result_of<F(Args...)>::type >;
			int size()
			{
				return task_size_;
			}
			void stoped()
			{
				{
					std::unique_lock<std::mutex> lock(queue_mutex);
					if (stop)
					{
						return;
					}
					stop = true;
				}
				condition.notify_all();
				for (size_t i = 0; i < workers.size(); ++i)
					workers[i].join();
			}

			~ThreadPool();
		private:
			// need to keep track of threads so we can join them
			std::vector< std::thread > workers;
			// the task queue
			std::queue< std::function<void()> > tasks;

			// synchronization
			std::mutex queue_mutex;
			std::condition_variable condition;
			std::condition_variable condition_enqueue_;
			bool stop;
			const size_t max_tasks_;
			std::atomic <int> task_size_ = { 0 };
		};

		// the constructor just launches some amount of workers
		inline ThreadPool::ThreadPool(const size_t threads)
			: stop(false)
			, max_tasks_(2 * threads)
		{
			for (size_t i = 0; i < threads; ++i)
				workers.emplace_back(
					[this] {
				for (;;) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->queue_mutex);
						while (!this->stop && this->tasks.empty())
							this->condition.wait(lock);
						if (this->stop && this->tasks.empty())
							return;
						task = this->tasks.front();
						this->tasks.pop();
					}
					task();
					task_size_--;
					condition_enqueue_.notify_one();
					/*if (tasks.size() == max_tasks_)
					{
					condition_enqueue_.notify_one();
					}*/
				}
			}
			);
		}

		// add new work item to the pool
		template<class F, class... Args>
		auto ThreadPool::enqueue(F&& f, Args&&... args)
			-> std::future < typename std::result_of<F(Args...)>::type > {
			typedef typename std::result_of<F(Args...)>::type return_type;

			// don't allow enqueueing after stopping the pool
			if (stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				tasks.push([task]() { (*task)(); });
				if (tasks.size() > max_tasks_)
				{
					condition_enqueue_.wait(lock);
				}
			}
			task_size_++;
			condition.notify_one();
			return res;
		}

		// the destructor joins all threads
		inline ThreadPool::~ThreadPool() {
			stoped();
		}
	}
}
