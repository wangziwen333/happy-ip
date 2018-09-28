#include <ip/server/tcp_session.h>
#include <ip/io_service_pool.h>

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			TcpSession::TcpSession(const bool is_closed_delay, const int session_timeout)
				: Session(session_timeout), is_closed_delay_(is_closed_delay), socket_(timer_.get_io_service()) {}

			void TcpSession::Start()
			{
				if (!message_factory_)
				{
					message_factory_ = make_shared <MessageFactory>();
				}
				ResetTimer();
				HandleCreate();
				DeliverRead();
				GetSessionId();
			}

			void TcpSession::DeliverRead()
			{
				socket_.async_read_some(buffer(buffer_), bind(&TcpSession::HandleRead, shared_from_this(), _1, _2));
			}

			void TcpSession::HandleRead(const boost::system::error_code& ec, const size_t bytes_recvd)
			{
				if (!ec) 
				{
					Session::HandleRead(buffer_.data(), bytes_recvd);
					DeliverRead();
				}
				else if (false == is_closed_delay_ && boost::asio::error::operation_aborted != ec)
				{
					Close(CLOSED_BY_REMOTE);
				}
			}

			void TcpSession::AsyncWrite(const string& packet, const string& tag)
			{
				shared_ptr <WriteData> write_data;
				if (async_write_buffer_.AsyncWrite(make_shared <WriteData>(packet, tag), write_data))
				{
					async_write(socket_, buffer(write_data->data), bind(&TcpSession::HandleWrite, shared_from_this(), _1, _2, write_data));
				}
			}

			void TcpSession::HandleWrite(const boost::system::error_code& ec, const size_t, shared_ptr <WriteData> in_write_data)
			{
				if (!ec)
				{
					message_factory_->HandleWrite(in_write_data->tag);
					shared_ptr <WriteData> write_data;
					if (async_write_buffer_.HandleWrite(write_data))
					{
						async_write(socket_, buffer(write_data->data), bind(&TcpSession::HandleWrite, shared_from_this(), _1, _2, write_data));
					}
					if (!is_handle_read_)
					{
						ResetTimer();
					}
				}
				else
				{
					async_write_buffer_.HandleWriteFailed(in_write_data);
				}
			}

			tcp::socket& TcpSession::GetSocket_()
			{
				return socket_;
			}

			string TcpSession::GetSessionId()
			{
				if (!session_id_.length())
				{
					boost::system::error_code ec, ec1;
					auto remote_endpoint = socket_.remote_endpoint(ec);
					auto local_endpoint = socket_.local_endpoint(ec1);
					if (!ec && !ec1)
					{
						session_id_ = local_endpoint.address().to_string() + ":" + to_string(local_endpoint.port()) + "_" 
							+ remote_endpoint.address().to_string() + ":" + to_string(remote_endpoint.port());
					}
				}
				return session_id_;
			}

			void TcpSession::ResetTimer()
			{
				if (session_timeout_)
				{
					timer_.cancel();
					timer_.expires_from_now(boost::posix_time::seconds(session_timeout_));
					timer_.async_wait(std::bind(&TcpSession::HandleSessionTimeout, shared_from_this(), _1));
				}
			}

			void TcpSession::HandleSessionTimeout(const boost::system::error_code& ec)
			{
				if (boost::asio::error::operation_aborted != ec)
				{
					Close(CLOSED_BY_TIMEOUT);
				}
			}

			void TcpSession::SyncWrite(const string& packet)
			{
				write(socket_, buffer(packet.data(), packet.length()));
			}

			void TcpSession::Close(const ClosedType closed_type)
			{
				Session::Close(closed_type);
				boost::system::error_code ec;
				socket_.cancel(ec);
				socket_.close(ec);
			}
		}
	}
}


