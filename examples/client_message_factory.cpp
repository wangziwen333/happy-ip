#include "client_message_factory.h"
#include <boost/asio.hpp>

bool ClientMessageFactory::IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package)
{
	uint32_t content_length = 0;
	if (length < 4 || (content_length = boost::asio::detail::socket_ops::network_to_host_long(*reinterpret_cast <const uint32_t *>(buffer))) > length - 4)
	{
		has_package = true;

		return false;
	}
	deal_length = 4 + content_length;

	return true;
}

bool ClientMessageFactory::HandleRead(const uint8_t* buffer, const size_t& length, string& response)
{
	printf("receive server data: %s\n", ToHost(buffer, length).content.c_str());

	return true;
}