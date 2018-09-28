#pragma once
#include <memory>
#include <google/protobuf/message.h>
using namespace google::protobuf;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			struct News
			{
				int session_id;
				shared_ptr <Message> message;
				News(const shared_ptr <Message> in_message = nullptr, const int in_session_id = 0)
				{
					message = in_message;
					session_id = in_session_id;
				}
			};
		}
	}
}
