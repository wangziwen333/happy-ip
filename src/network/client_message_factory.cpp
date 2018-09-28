#include <network/client_message_factory.h>
#include <network/proto_network_convert.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			ClientMessageFactory::ClientMessageFactory(const string& closed_print, const bool is_read_print)
				: closed_print_(closed_print), UtilMessageFactory(ProtoNetworkConvert::singleton::GetInstance(), is_read_print, false) {}

			void ClientMessageFactory::HandleClose(const ClosedType closed_type)
			{
				/*if (closed_print_.length())
				{
					LOG(WARNING) << closed_print_;
				}*/
			}
		}
	}
}
