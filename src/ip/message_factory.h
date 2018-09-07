#pragma once
#include <stdint.h>
#include <string>
#include <memory>
using namespace std;

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			enum ClosedType
			{
				CLOSED_BY_TIMEOUT,
				CLOSED_BY_REMOTE,
			};

			class MessageFactory
			{
			public:
				MessageFactory(const string& session_id = "")
					: session_id_(session_id)
				{}
				virtual shared_ptr <klicen::asio::ip::MessageFactory> Create(const string& session_id) 
				{ 
					return make_shared <klicen::asio::ip::MessageFactory>(session_id); 
				}
				virtual void HandleCreate(string& response){}
				virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) 
				{
					return true;
				}
				virtual void HandleClose(const ClosedType closed_type) {}
				virtual void HandleWrite(const string& tag) {};
				virtual bool IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package)
				{
					deal_length = length;

					return true;
				}
			protected:
				string session_id_;
			};
		}
	}
}