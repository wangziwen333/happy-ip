#pragma once
#include <ip/message_factory.h>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <ip/io_service_pool.h>
#include <ip/async_write_buffer.h>
#include <thread_pool.h>
using utils::ThreadPool;
using namespace std;
using namespace boost::asio;
using namespace std::placeholders;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			class Session
			{
			public:
				void SetMessageFactory(shared_ptr <happy::asio::ip::MessageFactory> message_factory);
				void SetCloseHandler(const function <void(const string&)>& handler);
				void SetMessageFactoryRead(const function <void(const string)>& handler);
				virtual void AsyncWrite(const string& buffer, const string& tag = "") = 0;
				void CancelTimer();
				Session(const int session_timeout)
					: session_timeout_(session_timeout) {}
				void MessageFactoryRead(const string str_buffer);
			protected:
				virtual void ResetTimer() = 0;
				virtual string GetSessionId();
				void HandleCreate();
				void HandleRead(const uint8_t* in_buffer, const size_t& in_buffer_length);
				void Close(const ClosedType closed_type);
			protected:
				shared_ptr <happy::asio::ip::MessageFactory> message_factory_;
				function <void(const string&)> close_handler_ { [&](const string&){} };
				AsyncWriteBuffer async_write_buffer_;
				deadline_timer timer_{ IoServicePool::singleton::GetInstance()->GetNextIoService_() };
				string receive_buffer_;
				const int session_timeout_; // unit: seconds
				bool is_handle_read_;
			private:
				function <void(const string)> message_factor_read_ = bind([this](const string str_buffer) { MessageFactoryRead(str_buffer); }, _1);
			};
		}
	}
}