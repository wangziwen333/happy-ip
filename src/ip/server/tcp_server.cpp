#include <ip/server/tcp_server.h>
#include <ip/io_service_pool.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			TcpServer::TcpServer(const uint16_t& port, const std::string &ip, const bool& is_closed_delay, const int& session_timeout, const bool& is_start_delay, const int read_work_threads)
				: Server(session_timeout), is_closed_delay_(is_closed_delay), is_start_delay_(is_start_delay)
				, acceptor_(IoServicePool::singleton::GetInstance()->GetNextIoService_(), tcp::endpoint(address::from_string(ip.c_str()), port))
			{
				if (read_work_threads)
				{
					read_thread_pool_ = make_shared <ThreadPool>(read_work_threads);
				}
				if (!is_start_delay)
					StartAccept();
			}

			void TcpServer::Run()
			{
				if (is_start_delay_)
					StartAccept();
			}

			void TcpServer::StartAccept()
			{
				shared_ptr<TcpSession> new_session(new TcpSession(is_closed_delay_, session_timeout_));
				acceptor_.async_accept(new_session->GetSocket_(), bind(&TcpServer::HandleAccept, this, new_session, _1));
			}

			void TcpServer::HandleAccept(const shared_ptr<TcpSession> session, const boost::system::error_code& ec)
			{
				if (!ec)
				{
					UpsertSession(session->GetSessionId(), session);
					auto message_factory = GetMessageFactory(session->GetSessionId());
					session->SetMessageFactory(message_factory);
					session->SetCloseHandler(CLOSE_HANDLE);
					if (read_thread_pool_ != nullptr)
					{
						session->SetMessageFactoryRead(bind([](const shared_ptr<TcpSession> session, shared_ptr <ThreadPool> read_thread_pool, const string str_buffer) {
							read_thread_pool->enqueue(&TcpSession::MessageFactoryRead, session->shared_from_this(), str_buffer);
						}, session, read_thread_pool_, _1));
					}
					session->Start();
				}
				StartAccept();
			}
		}
	}
}
