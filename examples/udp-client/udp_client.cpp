#include <ip/client/udp_client.h>
#include "client_message_factory.h"
#include <iostream>
#include <utils.h>
#include <thread>
#include <chrono>
using namespace std;
using namespace happy::asio::ip;

int main()
{
	// 默认创建线程池的大小为4。
	//IoServicePool::singleton::Create(4); 
	auto const ip = "127.0.0.1";
	auto const port = 1235;
	auto udp_client = make_shared <UdpClient>(ip, port);
	udp_client->SetMessageFactory(shared_ptr <happy::asio::ip::MessageFactory>(new ClientMessageFactory));
	thread t([]() { IoServicePool::singleton::GetInstance()->Run(); });
	t.detach();
	while (true)
	{
		cout << "please input send server string(enter to end): ";
		char buffer[1024];
		cin >> buffer;
		udp_client->AsyncWrite(ToNetwork(buffer));
		this_thread::sleep_for(chrono::seconds(1));
	}
	return EXIT_SUCCESS;
}