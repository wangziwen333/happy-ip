#pragma once
#include <ip/message_factory.h>
#include <utils.h>

class ClientMessageFactory : public happy::asio::ip::MessageFactory
{
	using happy::asio::ip::MessageFactory::MessageFactory;
	CREATE(ClientMessageFactory);
private:
	virtual bool IsConsumedForRead(const uint8_t* buffer, const size_t& length, size_t& deal_length, bool& has_package) override final;
	virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) override final;
};