#include <ip/server/udp_session.h>
#include <ip/io_service_pool.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			UdpSession::UdpSession(udp::socket& server_socket, const int& session_timeout)
				:  Session(session_timeout),server_socket_(server_socket)
			{
			}

			void UdpSession::AsyncWrite(const string& buffer, const string& tag)
			{
				if (buffer.length())
				{
					server_socket_.async_send_to(boost::asio::buffer(buffer.c_str(), buffer.length()), remote_endpoint_
						, bind(&UdpSession::HandleWrite, shared_from_this(), _1, _2));
				}
			}

			void UdpSession::HandleWrite(const boost::system::error_code& error, const size_t bytes_sent)
			{
				if (error)
				{
					return;
				}
			}

			udp::endpoint& UdpSession::GetRemoteEndpoint_()
			{
				return remote_endpoint_;
			}

			void UdpSession::HandleRead(const uint8_t* buffer, const size_t length)
			{
				ResetTimer();
				Session::HandleRead(buffer, length);
			}

			string UdpSession::GetSessionId()
			{
				return remote_endpoint_.address().to_string() + ":" + std::to_string(remote_endpoint_.port());
			}

			void UdpSession::ResetTimer()
			{
				timer_.cancel();
				timer_.expires_from_now(boost::posix_time::seconds(session_timeout_));
				timer_.async_wait(std::bind(&UdpSession::HandleSessionTimeout, shared_from_this(), _1));
			}

			void UdpSession::HandleSessionTimeout(const boost::system::error_code& ec)
			{
				if (boost::asio::error::operation_aborted != ec)
				{
					close_handler_(GetSessionId());
					message_factory_->HandleClose(CLOSED_BY_TIMEOUT);
				}
			}
		}
	}
}
