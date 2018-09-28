#pragma once
#include <google/protobuf/message.h>
#include <memory>
#include <network/news.h>
using namespace google::protobuf;
using namespace std;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class NetworkConvert
			{
			public:
				virtual bool IsConsumed(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& is_discarded) = 0;
				virtual shared_ptr <News> ToProto(const uint8_t* buffer, const size_t& length) = 0;
				virtual bool ToNetwork(const shared_ptr <Message>& message, string& out_buffer, const int& session_id = 0) = 0;
				shared_ptr <Message> CreateMessage(const string& type_name);
				string GetLastError();
			protected:
				void SetLastError(const string& last_error);
			private:
				string last_error_;
			};
		}
	}
}

