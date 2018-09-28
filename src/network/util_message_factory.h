#pragma once
#include <memory>
#include <ip/message_factory.h>
#include <network/news.h>
#include <google/protobuf/message.h>
#include <network/network_convert.h>
using namespace google::protobuf;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class UtilMessageFactory : public happy::asio::ip::MessageFactory
			{
			public:
				/*--------------------------------------------------------
				参数说明
					network_convert：网络层解析类
					is_read_print：是否打印读取到的消息
					is_response：是否应答数据（客户端与服务器端判别）
				--------------------------------------------------------*/
				UtilMessageFactory(NetworkConvert* network_convert, bool is_read_print, bool is_response, const string& session_id = "");
			protected:
				NetworkConvert* network_convert_;
				const bool is_read_print_;
				const bool is_response_;
			protected:
				virtual shared_ptr <Message> Produce(const shared_ptr <News> news) = 0;
				//virtual void PrintMessage(const shared_ptr<Message> message);
				virtual bool IsConsumedForRead(const uint8_t* buffer, const size_t& length
					, size_t& deal_length, bool& has_package) override final;
				virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) override final;
			};
		}
	}
}
