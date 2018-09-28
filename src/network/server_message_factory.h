#pragma once
#include <network/util_message_factory.h>
#include <network/proto_network_convert.h>
#include <map>
#include <functional>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class ServerMessageFactory : public UtilMessageFactory
			{
			protected:
				map <string, function <shared_ptr <Message>(const shared_ptr<Message>)>> handler_;
			public:
				ServerMessageFactory(const string& session_id = "", const bool is_read_print = false
					, NetworkConvert* network_convert = ProtoNetworkConvert::singleton::GetInstance());
			protected:
				virtual shared_ptr <Message> Produce(const shared_ptr <News> news) override;
			};
		}
	}
}

