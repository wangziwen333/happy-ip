#pragma once
#include <boost/asio.hpp>
#include <Ip/Server/Session.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			using namespace boost::asio::ip;
			using namespace std::placeholders;
			using namespace boost::asio;
			using namespace std;
			using namespace klicen::utils;

			class UdpSession : public enable_shared_from_this<UdpSession>, public Session
			{
			public:
				UdpSession(udp::socket& server_socket, const int session_timeout = 60);
				virtual void AsyncWrite(const string& buffer, const string& tag = "") override final;
				void HandleRead(const uint8_t* buffer, const size_t length);
				udp::endpoint& GetRemoteEndpoint_();
				virtual string GetSessionId();
			private:
				udp::socket& server_socket_;
			protected:
				udp::endpoint remote_endpoint_;
			private:
				void HandleWrite(const boost::system::error_code& error, const size_t bytes_sent);
				void ResetTimer();
				void HandleSessionTimeout(const boost::system::error_code& ec);
			};

		}
	}
}

