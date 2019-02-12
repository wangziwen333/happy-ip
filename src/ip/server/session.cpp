#include <ip/server/session.h>

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			void Session::SetMessageFactory(shared_ptr <happy::asio::ip::MessageFactory> message_factory)
			{
				message_factory_ = message_factory;
			}

			void Session::SetCloseHandler(const function <void(const string&)>& handler)
			{
				close_handler_ = handler;
			}

			void Session::SetMessageFactoryRead(const function <void(const string)>& handler)
			{
				message_factor_read_ = handler;
			}

			void Session::CancelTimer() 
			{ 
				timer_.cancel(); 
			}

			string Session::GetSessionId()
			{ 
				return ""; 
			}

			void Session::HandleCreate()
			{
				string response;
				message_factory_->HandleCreate(response);
				if (response.length())
				{
					AsyncWrite(response);
				}
			}

			void Session::MessageFactoryRead(const string str_buffer)
			{
				string response;
				if (message_factory_->HandleRead(reinterpret_cast <const uint8_t*>(str_buffer.data()), str_buffer.length(), response))
				{
					ResetTimer();
					if (response.length())
					{
						AsyncWrite(response);
					}
				}
			}

			void Session::HandleRead(const uint8_t* in_buffer, const size_t& in_buffer_length)
			{
				size_t total_deal_length = 0;
				const uint8_t* buffer = in_buffer;
				size_t buffer_length = in_buffer_length;
				is_handle_read_ = true;
				// continue package
				if (receive_buffer_.length())
				{
					receive_buffer_.append(reinterpret_cast <char*>(const_cast <uint8_t*>(in_buffer)), in_buffer_length);
					buffer = reinterpret_cast <const uint8_t *>(const_cast <char*>(receive_buffer_.data()));
					buffer_length = receive_buffer_.length();
				}
				while (total_deal_length < buffer_length)
				{
					size_t deal_length = 0;
					bool has_package = false;
					string response;
					if (message_factory_->IsConsumedForRead(buffer + total_deal_length, buffer_length - total_deal_length, deal_length, has_package))
					{
						string str_buffer;
						str_buffer.append(reinterpret_cast <const char*>(buffer + total_deal_length), deal_length);
						message_factor_read_(str_buffer);
						total_deal_length += deal_length;
					}
					else if (has_package)
					{
						receive_buffer_.assign(reinterpret_cast <char *>(const_cast <uint8_t*>(buffer)) + total_deal_length, buffer_length - total_deal_length);

						break;
					}
					else
					{
						total_deal_length += deal_length;
					}
					// ���û�к�����������ĳ�����Ϊ�㣬��ʾҵ���������⣬��ʱ����ȫ�����ݴ���
					if (deal_length <= 0 && !has_package)
					{
						break;
					}
				}
				if (total_deal_length == buffer_length)
				{
					receive_buffer_.clear();
				}
				is_handle_read_ = false;
			}

			void Session::Close(const ClosedType closed_type)
			{
				boost::system::error_code ec;
				timer_.cancel(ec);
				message_factory_->HandleClose(closed_type);
				close_handler_(GetSessionId());
				async_write_buffer_.Clear();
			}
		}
	}
}