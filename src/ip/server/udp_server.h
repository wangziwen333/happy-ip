#pragma once
#include <boost/asio.hpp>
#include <ip/server/udp_session.h>
#include <ip/server/server.h>
#include <string>
using namespace boost::asio::ip;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			class UdpServer : public server
			{
			public:
				UdpServer(const uint16_t& port, const std::string &ip = "0.0.0.0", const int& session_timeout = 0);

			private:
				// Receive socket
				udp::socket socket_;
				//shared_ptr <udp::socket> socket_;
				// Receive buffer
				array <uint8_t, 16 * 1024> buffer_;
				// Deliver receive message to boost io
				void DeliverReceive();
				// Receive handle
				void HandleReceive(const boost::system::error_code& error, const size_t bytes_recvd, shared_ptr<UdpSession> udp_session);
			};
		}
	}
}
