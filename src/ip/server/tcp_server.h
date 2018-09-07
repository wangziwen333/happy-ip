#pragma once
#include <stdint.h>
#include <ip/server/tcp_session.h>
#include <ip/server/server.h>
#include <ip/message_factory.h>
#include <string>
#include <thread_pool.h>
using namespace klicen::utils;

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			class TcpServer : public Server
			{
			public:
				TcpServer(const uint16_t& port, const std::string &ip = "0.0.0.0", const bool& is_closed_delay = false
					, const int& session_timeout = 0, const bool& is_start_delay = false, const int read_work_threads = 0);
				void Run();
			private:
				const bool is_closed_delay_;
				const bool is_start_delay_;
				shared_ptr <ThreadPool> read_thread_pool_ = nullptr;
			protected:
				tcp::acceptor acceptor_;

				virtual void StartAccept();
				virtual void HandleAccept(const shared_ptr<TcpSession> session, const boost::system::error_code& ec);
			};
		}
	}
}

