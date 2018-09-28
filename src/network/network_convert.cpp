#include "network_convert.h"

namespace happy
{
	namespace utils
	{
		namespace network
		{
			shared_ptr <Message> NetworkConvert::CreateMessage(const string& type_name)
			{
				if (!type_name.length())
				{
					return nullptr;
				}
				auto const descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
				if (descriptor)
				{
					auto const prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
					if (prototype)
					{
						return shared_ptr <Message>(prototype->New());
					}
				}
				return nullptr;
			}

			string NetworkConvert::GetLastError()
			{
				return last_error_;
			}

			void NetworkConvert::SetLastError(const string& last_error)
			{
				last_error_ = last_error;
			}
		}
	}
}