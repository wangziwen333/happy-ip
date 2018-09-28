#include <network/connect_message_factory.h>
#include <network/proto_network_convert.h>
using namespace std;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			ConnectMessageFactory::ConnectMessageFactory()
				: UtilMessageFactory(ProtoNetworkConvert::singleton::GetInstance(), true, false) {}

			bool ConnectMessageFactory::HandleRead(const uint8_t* buffer, const size_t& length, string& response)
			{
				auto news = ProtoNetworkConvert::singleton::GetInstance()->ToProto(buffer, length);
				if (nullptr == news)
				{
					return false;
				}
				//PrintMessage(news->message);
				ShowTips(response);

				return true;
			}

			void ConnectMessageFactory::HandleCreate(string& response)
			{
				ShowTips(response);
			}

			shared_ptr <Message> ConnectMessageFactory::Produce(const shared_ptr <News>)
			{
				return nullptr;
			}
		}
	}
}