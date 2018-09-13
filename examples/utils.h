#pragma once
#include <string>
#include <boost/asio.hpp>
using namespace std;

// 网络数据传输结构
struct NetworkData
{
	uint32_t length;
	string content;
};

#define CREATE(class_name); \
	private: \
	virtual shared_ptr <klicen::asio::ip::MessageFactory> Create(const string& session_id) override final { return make_shared <class_name>(session_id); }

inline string HexToString(const uint8_t* buffer, const size_t length, const string& separator = "")
{
	string result;
	auto GetChar = [](int value) {
		if (value < 10)
		{
			return value + 48;
		}
		else
		{
			return value + 65 - 10;
		}
	};
	for (size_t i = 0; i < length; i++)
	{
		if (result.length())
		{
			result.append(separator);
		}
		result.append(1, GetChar(buffer[i] >> 4));
		result.append(1, GetChar(buffer[i] % 16));
	}
	return result;
}

inline string ToNetwork(const string& content)
{
	NetworkData network_data;
	network_data.content = content;
	network_data.length = boost::asio::detail::socket_ops::network_to_host_long(network_data.content.length());
	string response;
	response.append(reinterpret_cast <char*>(&network_data.length), 4);
	response.append(network_data.content);

	return move(response);
}

inline NetworkData ToHost(const uint8_t* buffer, const size_t length)
{
	NetworkData network_data;
	network_data.length = boost::asio::detail::socket_ops::network_to_host_long(*reinterpret_cast <const int32_t*>(buffer));
	network_data.content.append(reinterpret_cast <const char*>(buffer + 4), network_data.length);

	return network_data;
}
