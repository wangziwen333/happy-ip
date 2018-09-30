#pragma once
#include <network/util_message_factory.h>
#include <network/proto_network_convert.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class ServerMessageFactory : public UtilMessageFactory
			{
			
			public:
				ServerMessageFactory(const string& session_id = "", const bool is_read_print = false
					, NetworkConvert* network_convert = ProtoNetworkConvert::singleton::GetInstance())
					: UtilMessageFactory(network_convert, is_read_print, true, session_id) {}
			};
		}
	}
}

