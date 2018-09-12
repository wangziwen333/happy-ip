#pragma once
#include <ip/message_factory.h>
#include <utils.h>

class ServerMessageFactory : public klicen::asio::ip::MessageFactory
{
	using klicen::asio::ip::MessageFactory::MessageFactory;
	CREATE(ServerMessageFactory);
private:
	virtual bool IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package) override final;
	virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) override final;
};