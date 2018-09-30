#include "proto_server_message_factory.h"
#include "dmo.pb.h"

ProtoServerMessageFactory::ProtoServerMessageFactory(const string& session_id, const bool is_read_print)
	: ServerMessageFactory(session_id, is_read_print)
{
	handler_[RequestInfo::descriptor()->full_name()] = std::bind(&ProtoServerMessageFactory::RequestInfoHandler, this, std::placeholders::_1);
}

shared_ptr <Message> ProtoServerMessageFactory::RequestInfoHandler(const shared_ptr<Message> message)
{
	//auto request_info = dynamic_pointer_cast <RequestInfo>(message);
	auto response = make_shared <ResponseInfo>();
	response->set_info("server response data");

	return response;
}