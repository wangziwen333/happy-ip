#ifndef _KLICEN_UTILS_SERVER_IO_SERVICE_POOL_H_
#define _KLICEN_UTILS_SERVER_IO_SERVICE_POOL_H_

#include <boost/asio.hpp>
#include <singleton.h>
#include <mutex>
#include <string>
#include <vector>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{

			class IoServicePool
			{
				DEFINE_SINGLETON_CONSTRUCT_WITH_DEFAULT(IoServicePool);
			public:
				/// Construct the io_service pool.
				IoServicePool(const size_t pool_size = 4);

				/// Run all io_service objects in the pool.
				void Run();

				/// Stop all io_service objects in the pool.
				void Stop();

				/// Get an io_service to use.
				boost::asio::io_service& GetNextIoService_();

			private:
				typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
				typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

				/// The pool of io_services.
				std::vector<io_service_ptr> io_services_;

				/// The work that keeps the io_services running.
				std::vector<work_ptr> work_;

				/// The next io_service to use for a connection.
				std::size_t next_io_service_ = 0;

				std::mutex mutex_;
			};

		}
	}
}

#endif