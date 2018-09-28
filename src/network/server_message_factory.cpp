#include <network/server_message_factory.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			ServerMessageFactory::ServerMessageFactory(const string& session_id, const bool is_read_print, NetworkConvert* network_convert)
				: UtilMessageFactory(network_convert, is_read_print, true, session_id) {}

			shared_ptr <Message> ServerMessageFactory::Produce(const shared_ptr <News> news)
			{
				auto it = handler_.find(news->message->GetDescriptor()->full_name());
				if (handler_.end() == it)
				{
					return nullptr;
				}
				return it->second(news->message);
			}
		}
	}
}