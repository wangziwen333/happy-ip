#include <ip/server/udp_server.h>
#include <ip/io_service_pool.h>
using namespace boost::asio;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			UdpServer::UdpServer(const uint16_t& port, const std::string &ip, const int& session_timeout)
				: server(session_timeout)
				, socket_(IoServicePool::singleton::GetInstance()->GetNextIoService_(), udp::endpoint(address::from_string(ip.c_str()), port))
			{
				DeliverReceive();
			}

			void UdpServer::DeliverReceive()
			{
				shared_ptr<UdpSession> udp_session(new UdpSession(socket_, session_timeout_));
				socket_.async_receive_from(buffer(buffer_), udp_session->GetRemoteEndpoint_(), bind(&UdpServer::HandleReceive, this, _1, _2, udp_session));
			}

			void UdpServer::HandleReceive(const boost::system::error_code& error, const size_t bytes_recvd, shared_ptr <UdpSession> udp_session)
			{
				string response;
				if (!error)
				{
					auto session = server::FindSession(udp_session->GetSessionId());
					if (nullptr == session)
					{
						server::UpsertSession(udp_session->GetSessionId(), udp_session);
						auto message_factory = GetMessageFactory(udp_session->GetSessionId());
						udp_session->SetMessageFactory(message_factory);
						udp_session->SetCloseHandler(CLOSE_HANDLE);
						session = udp_session;
					}
					dynamic_cast <UdpSession*>(session.get())->HandleRead(buffer_.data(), bytes_recvd);
				}
				DeliverReceive();
			}
		}
	}
}
