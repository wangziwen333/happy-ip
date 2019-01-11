#pragma once
#include <stdint.h>
#include <ip/server/tcp_session.h>
#include <ip/server/server.h>
#include <ip/message_factory.h>
#include <string>
#include <thread_pool.h>
using utils::ThreadPool;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			class TcpServer : public server
			{
			public:
				/*--------------------------------------------------------
				参数
					port：绑定的端口
					ip：绑定的ip地址
					is_closed_delay：是否关闭延迟（如果设置此参数，只有session_timeout到期时，业务层才会收到close事件）
					session_timeout：会话超时时间（每个包之间的最长间隔时间，超时会自动关闭会话）
					is_start_delay：启动延迟（如果设置此参数，要手动运行run函数，服务器才会启动）
					read_work_threads：投递读事件线程池大小（设置此参数后，底层会自动将收到的数据投递到线程池中处理，防止业务处理阻塞读事件）
				--------------------------------------------------------*/
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

