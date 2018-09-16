#pragma once
#include <ip/server/udp_session.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			class UdpClient : public UdpSession
			{
			//public:
			//	/*--------------------------------------------------------
			//	参数
			//		auto_reconnect：自动重连标识（设置此标识，客户端会在检查到连接关闭时，重新发起异步连接）
			//		session_timeout：会话超时时间
			//	--------------------------------------------------------*/
			//	TcpClient(const bool& auto_reconnect = true, const int& session_timeout = 0);
			//	void AsyncConnect(const string& ip_addr, const int& port);
			//	bool Connect(const string& ip_addr, const int& port, const int& waiting_seconds = 0);
			//	void Disconnect();
			//private:
			//	void DeliverConnect(const int seconds = 1);
			//	void HandleConnect(const boost::system::error_code& ec);
			//	bool Reconnect(const int& waiting_seconds);
			//	virtual void Close(const ClosedType closed_type) override final;
			//	void AsyncConnectHandler(const boost::system::error_code&);
			//private:
			//	tcp::endpoint endpoint_;
			//	deadline_timer timer_reconnect_;
			public:
				/*--------------------------------------------------------
				参数
				    ip_addr：udp服务器ip地址
					port：udp服务器端口
					session_timeout：会话超时时间
				--------------------------------------------------------*/
				UdpClient(const string& ip_addr, const int port, const int session_timeout = 0);
				void DeliverReceive();
				void HandleReceive(const boost::system::error_code& error, const size_t bytes_recvd);
				//void Test()
				//{
				//	ip::udp::endpoint local_endpoint(ip::udp::v4(), 7777);
				//	ip::udp::endpoint remote_endpoint(ip::address_v4::from_string("127.0.0.1"), 1235);
				//	ip::udp::socket socket(IoServicePool::singleton::GetInstance()->GetNextIoService_()/*, local_endpoint*/);
				//	socket.open(udp::v4());
				//	char *send_data = "hello! my name is Bojie. Can you see me?";
				//	try
				//	{
				//		socket.send_to(buffer(send_data, strlen(send_data) + 1/*the size of contents*/), remote_endpoint);
				//	}
				//	catch (std::exception& e)
				//	{
				//		printf("%s", e.what());
				//	}
				//	catch (...){}
				//	
				//	printf("");
				//}
			private:
				udp::socket socket_;
				// Receive buffer
				array <uint8_t, 16 * 1024> buffer_;
			};
		}
	}
}

