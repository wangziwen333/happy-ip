#include <ip/client/tcp_client.h>
#include <thread>
using namespace std::chrono;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			TcpClient::TcpClient(const bool auto_reconnect, const int session_timeout)
				: TcpSession(false, session_timeout) , timer_reconnect_(timer_.get_io_service())
			{
				reconnect_ = auto_reconnect;
				if (auto_reconnect)
				{
					SetCloseHandler([&](const string&)
					{
						if (reconnect_)
						{
							DeliverConnect();
						}
					});
				}
			}

			void TcpClient::AsyncConnect(const string& ip_addr, const int port)
			{
				endpoint_ = tcp::endpoint(address::from_string(ip_addr.c_str()), port);
				DeliverConnect(0);
			}

			void TcpClient::DeliverConnect(const int seconds)
			{
				timer_reconnect_.expires_from_now(boost::posix_time::seconds(seconds));
				timer_reconnect_.async_wait(std::bind(&TcpClient::AsyncConnectHandler, this, _1));
			}

			void TcpClient::AsyncConnectHandler(const boost::system::error_code&)
			{
				socket_.async_connect(endpoint_, bind(&TcpClient::HandleConnect, this, _1));
			}

			void TcpClient::HandleConnect(const boost::system::error_code& ec)
			{
				if (!ec)
				{
					Start();
				}
				else if (ec != boost::asio::error::already_connected)
				{
					DeliverConnect();
				}
			}

			bool TcpClient::Connect(const string& ip_addr, const int port, const int waiting_seconds)
			{
				endpoint_ = tcp::endpoint(address::from_string(ip_addr.c_str()), port);

				return Reconnect(waiting_seconds);
			}

			void TcpClient::Disconnect()
			{
				reconnect_ = false;
				timer_reconnect_.cancel();
				Close(CLOSED_BY_REMOTE);
			}

			bool TcpClient::Reconnect(const int waiting_seconds)
			{
				if (!endpoint_.port())
				{
					return false;
				}

				auto start_time = system_clock::now();
				boost::system::error_code error = boost::asio::error::host_not_found;
				duration<double> elapsed_seconds;
				while (1)
				{
					// Has error is true
					if (socket_.connect(endpoint_, error))
					{
						elapsed_seconds = system_clock::now() - start_time;
						if (elapsed_seconds.count() > waiting_seconds)
						{
							return false;
						}
						continue;
					}
					else
					{
						break;
					}
				}
				Start();

				return true;
			}

			void TcpClient::Close(const ClosedType closed_type)
			{
				boost::system::error_code ec;
				socket_.cancel(ec);
				socket_.close(ec);
				Session::Close(closed_type);
			}
		}
	}
}
