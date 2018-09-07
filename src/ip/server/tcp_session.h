#pragma once
#include <boost/asio.hpp>
#include <ip/message_factory.h>
#include <string>
#include <ip/server/session.h>
#include <ip/async_write_buffer.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			using namespace boost::asio;
			using namespace std::placeholders;
			using namespace std;
			using namespace boost::asio::ip;

			class TcpSession : public enable_shared_from_this<TcpSession>, public Session
			{
			public:
				TcpSession(const bool& is_closed_delay = false, const int& session_timeout = 60);
				void Start();
				virtual void AsyncWrite(const string& packet, const string& tag = "") override final;
				void SyncWrite(const string& packet);
				tcp::socket& GetSocket_();
				virtual string GetSessionId();
			protected:
				const bool is_closed_delay_;
				tcp::socket socket_;
				array<uint8_t, 16 * 1024> buffer_;
				string session_id_ = "";
			protected:
				// Release server resource
				void DeliverRead();
				void HandleRead(const boost::system::error_code& ec, const size_t bytes_recvd);
				void HandleWrite(const boost::system::error_code& ec, const size_t&, shared_ptr <WriteData> in_write_data);
				virtual void ResetTimer() override final;
				void HandleSessionTimeout(const boost::system::error_code& ec);
				virtual void Close(const ClosedType closed_type);
			};
		}
	}
}

