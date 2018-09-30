#pragma once
#include <network/server_message_factory.h>
#include <utils.h>
using namespace happy::utils::network;

class ProtoServerMessageFactory : public ServerMessageFactory
{
	CREATE_PROTO(ProtoServerMessageFactory);
public:
	ProtoServerMessageFactory(const string& session_id, const bool is_read_print);
private:
	shared_ptr <Message> RequestInfoHandler(const shared_ptr<Message> message);
};