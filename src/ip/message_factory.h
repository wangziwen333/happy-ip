#pragma once
#include <stdint.h>
#include <string>
#include <memory>
using namespace std;

namespace happy
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
				virtual shared_ptr <happy::asio::ip::MessageFactory> Create(const string& session_id) 
				{ 
					return make_shared <happy::asio::ip::MessageFactory>(session_id); 
				}
				virtual void HandleCreate(string& response){}
				/*--------------------------------------------------------
				参数
					buffer：网络数据缓冲区
					length：网络数据长度
					response：应答数据缓冲区
				返回值
					true：业务层处理数据包成功（重新调整会话的超时时间）
					false：业务层处理数据包失败
				--------------------------------------------------------*/
				virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) 
				{
					return true;
				}
				virtual void HandleClose(const ClosedType closed_type) {}
				virtual void HandleWrite(const string& tag) {};
				/*--------------------------------------------------------
				参数
					buffer：网络数据缓冲区
					length：网络数据长度
					deal_length：已处理网络数据长度
					has_package：是否有后续数据包   
				返回值
					true：数据包处理成功（收到一个完整的包）
					false：数据处理失败（has_package为true表示收到一个不完整的包、has_package为false表示需要跳过deal_length的数据包）
				--------------------------------------------------------*/ 
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