#pragma once
#include <ip/server/tcp_session.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			class TcpClient : public TcpSession
			{
			public:
				/*--------------------------------------------------------
				参数
					auto_reconnect：自动重连标识（设置此标识，客户端会在检查到连接关闭时，重新发起异步连接）
					session_timeout：会话超时时间
				--------------------------------------------------------*/
				TcpClient(const bool auto_reconnect = true, const int session_timeout = 0);
				void AsyncConnect(const string& ip_addr, const int port);
				bool Connect(const string& ip_addr, const int port, const int waiting_seconds = 0);
				void Disconnect();
			private:
				void DeliverConnect(const int seconds = 1);
				void HandleConnect(const boost::system::error_code& ec);
				bool Reconnect(const int waiting_seconds);
				virtual void Close(const ClosedType closed_type) override final;
				void AsyncConnectHandler(const boost::system::error_code&);
			private:
				tcp::endpoint endpoint_;
				deadline_timer timer_reconnect_;
				volatile bool reconnect_;
			};
		}
	}
}

