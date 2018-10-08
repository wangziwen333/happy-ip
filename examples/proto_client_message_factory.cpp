#include "proto_client_message_factory.h"
#include "dmo.pb.h"

ProtoClientMessageFactory::ProtoClientMessageFactory(const string& closed_print, const bool is_read_print)
	: ClientMessageFactory(closed_print, is_read_print)
{
	handler_[RequestInfo::descriptor()->full_name()] = std::bind(&ProtoClientMessageFactory::ResponseInfoHandler, this, std::placeholders::_1);
}

shared_ptr <Message> ProtoClientMessageFactory::Produce(const shared_ptr <News> news)
{
	return nullptr;
}

shared_ptr <Message> ProtoClientMessageFactory::ResponseInfoHandler(const shared_ptr<Message> message)
{
	//auto response_info = dynamic_pointer_cast <ResponseInfo>(message);
	return nullptr;
}