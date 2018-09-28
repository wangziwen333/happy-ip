#include <network/proto_network_convert.h>
#include <network/adler32.h>
#include <network/quicklz.h>
#include <boost/asio.hpp>
using namespace boost::asio::detail::socket_ops;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			bool ProtoNetworkConvert::IsConsumed(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package)
			{
				deal_length = length;
				if (length < sizeof(NetworkHeader))
				{
					has_package = true;

					return false;
				}
				NetworkData network_data{ 0 };
				network_data.header.MemoryCopyAndConvert(buffer);
				auto total_length = sizeof(network_data.header) + network_data.header.content_length + network_data.header.message_name_length + sizeof(network_data.check_sum);
				// header check sum
				if (adler32(buffer, sizeof(network_data.header) - sizeof(network_data.header.check_sum)) != network_data.header.check_sum)
				{
					return false;
				}
				// has package
				if (total_length > length)
				{
					has_package = true;

					return false;
				}

				deal_length = total_length;
				// check sum
				if (adler32(buffer, total_length - sizeof(network_data.check_sum)) 
					!= network_to_host_long(*(reinterpret_cast <uint32 *>(const_cast <uint8_t *>(buffer + total_length - sizeof(network_data.check_sum))))))
				{
					return false;
				}
				return true;
			}

			shared_ptr <News> ProtoNetworkConvert::ToProto(const uint8_t* buffer, const size_t&)
			{
				shared_ptr <News> news = make_shared <News>();
				NetworkData network_data{ 0 };
				network_data.header.MemoryCopyAndConvert(buffer);
				network_data.message_name.append(reinterpret_cast <char *>(const_cast <uint8_t *>(buffer)) + sizeof(NetworkHeader), network_data.header.message_name_length);
				network_data.content.append(reinterpret_cast <char *>(const_cast <uint8_t *>(buffer)) + sizeof(NetworkHeader) + network_data.header.message_name_length
					, network_data.header.content_length);
				if (network_data.header.content_compress_type)
				{
					auto length = qlz_size_decompressed(network_data.content.c_str());
					if (length > 102400000)
					{
						length = network_data.content.length() * 10;
					}
					string str(length, '\0');
					str.resize(qlz_decompress(network_data.content.c_str(), const_cast <char *>(str.data()), &qlz_state_decompress()));
					network_data.content.swap(str);
				}
				news->session_id = network_data.header.session_id;
				news->message = CreateMessage(network_data.message_name);

				if (nullptr == news->message)
				{
					SetLastError("Create message "+ network_data.message_name +" error!");
					return news;
				}

				if (!news->message->ParseFromString(network_data.content))
				{
					SetLastError("Parse message " + network_data.message_name + " error!");
					news->message = nullptr;
					return news;
				}
				if (!news->message->IsInitialized())
				{
					SetLastError("Initialize message " + network_data.message_name + " error!");
					news->message = nullptr;
					return news;
				}

				return news;
				
			}

			bool ProtoNetworkConvert::ToNetwork(const shared_ptr <Message>& message, string& out_buffer, const int& session_id)
			{
				if (nullptr == message)
				{
					return false;
				}
				NetworkData network_data;
				static const int kMaxUncompressSize = 1024000;
				auto conent = message->SerializeAsString();
				if (conent.length() > kMaxUncompressSize)
				{
					network_data.content.resize(conent.length());
					network_data.content.resize(qlz_compress(conent.c_str(), const_cast <char *>(network_data.content.data()), conent.length(), &qlz_state_compress()));
					network_data.header.content_compress_type = 1;
				}
				else
				{
					network_data.header.content_compress_type = 0;
					network_data.content.swap(conent);
				}
				//network_data.content = message->SerializeAsString();
				network_data.message_name = message->GetDescriptor()->full_name();
				network_data.header.session_id = session_id;
				network_data.header.content_length = network_data.content.length();
				network_data.header.message_name_length = network_data.message_name.length();
				network_data.header.MemoryCopyAndConvert(nullptr);
				network_data.header.check_sum = network_to_host_long(adler32(reinterpret_cast <char *>(&network_data.header)
					, sizeof(network_data.header) - sizeof(network_data.header.check_sum)));
				out_buffer = "";
				out_buffer.append(reinterpret_cast <char *>(&network_data.header), sizeof(network_data.header));
				out_buffer.append(network_data.message_name);
				out_buffer.append(network_data.content);
				network_data.check_sum = network_to_host_long(adler32(out_buffer.c_str(), out_buffer.length()));
				out_buffer.append(reinterpret_cast <char *>(&network_data.check_sum), sizeof(network_data.check_sum));

				return true;
			}
		}
	}
}
