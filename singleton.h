#pragma once
#include <mutex>

namespace happy 
{
	namespace utils 
	{
		class noncopyable
		{
		protected:
			noncopyable() = default;
			~noncopyable() = default;
			noncopyable(const noncopyable&) = delete;
			noncopyable& operator=(const noncopyable&) = delete;
		};

		template <typename T>
		class Singleton : private noncopyable
		{
		public:
			static T* GetInstance()
			{
				// double check
				if (nullptr == instance_)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (nullptr == instance_)
					{
						instance_ = new T;
					}
				}
				return instance_;
			}
		private:
			Singleton();
			static T* instance_;
			static std::mutex mutex_;
		};
		template <typename T> T* Singleton<T>::instance_ = nullptr;
		template <typename T> std::mutex Singleton<T>::mutex_;
#define DEFINE_SINGLETON_DEFAULT(class_name); \
	public: \
	friend class happy::utils::Singleton<class_name>; \
	typedef happy::utils::Singleton<class_name> singleton; \
	private: \
	class_name() {} \
	virtual ~class_name() {} \
	class_name(const class_name&){} \
	class_name& operator=(const class_name&)= delete;

		template <typename T>
		class SingletonConstruct : private noncopyable
		{
		public:
			static T* GetInstance()
			{
				/*if (nullptr == instance_)
				{
				std::lock_guard<std::mutex> lock(mutex_);
				if (nullptr == instance_)
				{
				throw std::exception("Get singleton failed");
				}
				}*/
				return instance_;
			}
			template <typename... Args>
			static void Create(Args&&... args)
			{
				if (nullptr == instance_)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (nullptr == instance_)
					{
						instance_ = new T(std::forward<Args>(args)...);
					}
				}
			}
		private:
			SingletonConstruct();
			static T* instance_;
			static std::mutex mutex_;
		};
		template <typename T> T* SingletonConstruct<T>::instance_ = nullptr;
		template <typename T> std::mutex SingletonConstruct<T>::mutex_;
#define DEFINE_SINGLETON_CONSTRUCT(class_name); \
	public: \
	friend class happy::utils::SingletonConstruct<class_name>; \
	typedef happy::utils::SingletonConstruct<class_name> singleton; \
	private: \
	virtual ~class_name() {} \
	class_name& operator=(const class_name&)= delete;

		template <typename T>
		class SingletonConstructNoParam : private noncopyable
		{
		public:
			static T* GetInstance()
			{
				if (nullptr == instance_)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (nullptr == instance_)
					{
						instance_ = new T;
					}
				}
				return instance_;
			}
		private:
			SingletonConstructNoParam();
			static T* instance_;
			static std::mutex mutex_;
		};
		template <typename T> T* SingletonConstructNoParam<T>::instance_ = nullptr;
		template <typename T> std::mutex SingletonConstructNoParam<T>::mutex_;

#define DEFINE_SINGLETON_CONSTRUCT_NO_PARAM(class_name); \
	public: \
	friend class happy::utils::SingletonConstructNoParam<class_name>; \
	typedef happy::utils::SingletonConstructNoParam<class_name> singleton; \
	private: \
	class_name(); \
	class_name& operator=(const class_name&) = delete; 

		template <typename T>
		class SingletonConstructWithDefault : private noncopyable
		{
		public:
			static T* GetInstance()
			{
				if (nullptr == instance_)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (nullptr == instance_)
					{
						instance_ = new T;
					}
				}
				return instance_;
			}
			template <typename... Args>
			static void Create(Args&&... args)
			{
				if (nullptr == instance_)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (nullptr == instance_)
					{
						instance_ = new T(std::forward<Args>(args)...);
					}
				}
			}
		private:
			SingletonConstructWithDefault();
			static T* instance_;
			static std::mutex mutex_;
		};
		template <typename T> T* SingletonConstructWithDefault<T>::instance_ = nullptr;
		template <typename T> std::mutex SingletonConstructWithDefault<T>::mutex_;
#define DEFINE_SINGLETON_CONSTRUCT_WITH_DEFAULT(class_name); \
	public: \
	friend class happy::utils::SingletonConstructWithDefault<class_name>; \
	typedef happy::utils::SingletonConstructWithDefault<class_name> singleton; \
	private: \
	virtual ~class_name() {} \
	class_name& operator=(const class_name&) = delete;
	} // namespace utils
} // namespace happy
