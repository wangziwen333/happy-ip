#pragma once
#include <singleton.h>
#include <network/network_convert.h>
using namespace happy::utils;

#define DEFINE_HOST_NETWORK_CONVERT(format) \
void MemoryCopyAndConvert(const uint8_t* buffer) \
							{ \
	if (nullptr == format) \
							{ \
		return; \
							} \
	auto tmp_buffer = *this; \
	const uint8_t* current_buffer = buffer; \
	if (nullptr == current_buffer) \
						{ \
		current_buffer = reinterpret_cast <uint8_t *>(&tmp_buffer); \
			} \
		int member_size; \
		uint8_t* current_data = reinterpret_cast <uint8_t *>(this); \
		for (size_t i = 0; i < strlen(format); i++) \
						{ \
		member_size = format[i] - '0'; \
		if (2 == member_size) \
						{ \
		current_data[0] = current_buffer[1]; \
		current_data[1] = current_buffer[0]; \
						} \
								else if (4 == member_size) \
					{ \
		current_data[0] = current_buffer[3]; \
		current_data[1] = current_buffer[2]; \
		current_data[2] = current_buffer[1]; \
		current_data[3] = current_buffer[0]; \
					} \
								else if (8 == member_size) \
									{ \
		current_data[0] = current_buffer[7]; \
		current_data[1] = current_buffer[6]; \
		current_data[2] = current_buffer[5]; \
		current_data[3] = current_buffer[4]; \
		current_data[4] = current_buffer[3]; \
		current_data[5] = current_buffer[2]; \
		current_data[6] = current_buffer[1]; \
		current_data[7] = current_buffer[0]; \
									} \
								else \
					{ \
		current_data[0] = current_buffer[0]; \
					} \
		current_data += member_size; \
		current_buffer += member_size; \
						} \
				}

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class ProtoNetworkConvert : public NetworkConvert
			{
				DEFINE_SINGLETON_DEFAULT(ProtoNetworkConvert);
			public:
				virtual bool IsConsumed(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package) override final;
				virtual shared_ptr <News> ToProto(const uint8_t* buffer, const size_t& length) override final;
				virtual bool ToNetwork(const shared_ptr <Message>& message, string& out_buffer, const int& session_id = 0) override final;
			private:
#pragma pack(push, 1)
				struct NetworkHeader
				{
					int session_id; // 会话标识
					int message_name_length; // 消息名称长度
					int content_length; // 消息内容长度
					uint8_t content_compress_type; // 0: 未压缩; 1: klz压缩
					uint32_t check_sum; // 头校验和
					DEFINE_HOST_NETWORK_CONVERT("44414");
				};
#pragma pack(pop)
				struct NetworkData
				{
					NetworkHeader header; // 消息头
					string message_name; // 消息名称
					string content; // 消息内容
					uint32_t check_sum; // 校验和
				};
			};
		}
	}
}

