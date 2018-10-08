#pragma once
#include <network/client_message_factory.h>
#include <utils.h>
using namespace happy::utils::network;

class ProtoClientMessageFactory : public ClientMessageFactory
{
public:
	ProtoClientMessageFactory(const string& closed_print = "", const bool is_read_print = false);
private:
	virtual shared_ptr <Message> Produce(const shared_ptr <News> news) override final;
	shared_ptr <Message> ResponseInfoHandler(const shared_ptr<Message> message);
};