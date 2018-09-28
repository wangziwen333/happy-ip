#include <ip/io_service_pool.h>
#include <mutex>
#include <thread>
#include <functional>
#include <boost/thread.hpp>
using namespace std;
using namespace boost::asio;

namespace happy
{
	namespace asio
	{
		namespace ip
		{

			void IoServicePool::Run()
			{
				// Create a pool of threads to run all of the io_services.
				std::vector<boost::shared_ptr<boost::thread>> threads;
				for (std::size_t i = 0; i < io_services_.size(); ++i)
				{
					boost::shared_ptr<boost::thread> thread(new boost::thread(bind(static_cast<std::size_t(io_service::*)()>(&io_service::run), io_services_[i])));
					threads.push_back(thread);
				}

				// Wait for all threads in the pool to exit.
				for (auto& thread : threads)
					thread->join();
			}

			void IoServicePool::Stop()
			{
				// Explicitly stop all io_services.
				for (auto& io_service : io_services_)
					io_service->stop();
			}

			io_service& IoServicePool::GetNextIoService_()
			{
				std::lock_guard<std::mutex> lock(mutex_);
				// Use a round-robin scheme to choose the next io_service to use.
				io_service& io_service = *io_services_[next_io_service_];
				next_io_service_ = (next_io_service_ + 1) % io_services_.size();

				return io_service;
			}

			IoServicePool::IoServicePool(const size_t pool_size)
			{
				for (std::size_t i = 0; i < pool_size; ++i)
				{
					io_service_ptr service(new io_service);
					work_ptr work(new io_service::work(*service));
					io_services_.push_back(service);
					work_.push_back(work);
				}
			}
		}
	}
}
