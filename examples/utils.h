#include <string>
using namespace std;

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
