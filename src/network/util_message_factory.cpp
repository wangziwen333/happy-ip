#include <network/util_message_factory.h>
#include <network/output.h>
#include <network/response_error.pb.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			UtilMessageFactory::UtilMessageFactory(NetworkConvert* network_convert, bool is_read_print, bool is_response, const string& session_id)
				: happy::asio::ip::MessageFactory(session_id), network_convert_(network_convert), is_read_print_(is_read_print), is_response_(is_response) {}

			/*void UtilMessageFactory::PrintMessage(const shared_ptr<Message> message)
			{
				if (is_read_print_)
				{
					OUTPUT << "receive message: " << message->GetDescriptor()->full_name();
					if (message->ByteSize() && message->ByteSize() < 1024)
					{
						OUTPUT << message->DebugString() << endl;
					}
				}
			}*/

			bool UtilMessageFactory::IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package)
			{
				return network_convert_->IsConsumed(buffer, length, deal_length, has_package);
			}

			bool UtilMessageFactory::HandleRead(const uint8_t* buffer, const size_t& length, string& response)
			{
				auto news = network_convert_->ToProto(buffer, length);
				shared_ptr <Message> response_message = nullptr;
				if (nullptr == news)
				{
					return false;
				}
				do
				{
					if (nullptr == news->message)
					{
						auto response_error = make_shared <ResponseError>();
						response_error->set_error_code(9);
						response_error->set_error_string(network_convert_->GetLastError());
						response_message = response_error;
					}
					else
					{
						//PrintMessage(news->message);
						response_message = Produce(news);
						if (!is_response_)
						{
							break;
						}
						if (nullptr == response_message)
						{
							auto response_error = make_shared <ResponseError>();
							response_error->set_error_code(1);
							response_error->set_error_string("Unknown error");
							response_message = response_error;
						}
					}
					if (is_read_print_)
					{
						OUTPUT << "Send response: " << response_message->GetDescriptor()->full_name() << endl;
					}
					network_convert_->ToNetwork(response_message, response, news->session_id);
				} while (false);

				return true;
			}
		}
	}
}

