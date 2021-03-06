#include <ip/client/udp_client.h>
#include "proto_client_message_factory.h"
#include <iostream>
#include <utils.h>
#include <thread>
#include <chrono>
#include "dmo.pb.h"
#include <network/proto_network_convert.h>
#ifdef GLOG_OUTPUT
#include <config_glog.h>
#endif
using namespace happy::utils::network;
using namespace std;
using namespace happy::asio::ip;

int main(int argc, char* argv[])
{
	// 默认创建线程池的大小为4
	//IoServicePool::singleton::Create(4); 
#ifdef GLOG_OUTPUT
	utils::ConfigGlog(argv[0]);
#endif
	
	auto const ip = "127.0.0.1";
	auto const port = 1235;
	auto udp_client = make_shared <UdpClient>(ip, port);
	udp_client->SetMessageFactory(dynamic_pointer_cast <happy::asio::ip::MessageFactory>(make_shared <ProtoClientMessageFactory>("", true)));
	thread t([]() { IoServicePool::singleton::GetInstance()->Run(); });
	t.detach();
	while (true)
	{
		cout << "please input send server string(enter to end): ";
		char buffer[1024];
		cin >> buffer;
		auto request_info = make_shared <RequestInfo>();
		request_info->set_info(buffer);
		string out_buffer;
		ProtoNetworkConvert::singleton::GetInstance()->ToNetwork(request_info, out_buffer);
		udp_client->AsyncWrite(out_buffer);
		this_thread::sleep_for(chrono::seconds(1));
	}
	return EXIT_SUCCESS;
}