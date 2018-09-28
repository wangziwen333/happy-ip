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
				UtilMessageFactory(NetworkConvert* network_convert, bool is_read_print, bool is_response, const string& session_id = "");
			protected:
				NetworkConvert* network_convert_;
				const bool is_read_print_;
				const bool is_response_;
			protected:
				virtual shared_ptr <Message> Produce(const shared_ptr <News> news) = 0;
				//virtual void PrintMessage(const shared_ptr<Message> message);
				virtual bool IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package);
				virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) override;
			};
		}
	}
}
