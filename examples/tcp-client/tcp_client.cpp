#include <ip/client/tcp_client.h>
#include "client_message_factory.h"
#include <iostream>
#include <utils.h>
#include <thread>
#include <chrono>
using namespace std;
using namespace klicen::asio::ip;

int main()
{
	// 默认创建线程池的大小为4。
	//IoServicePool::singleton::Create(4); 
	auto tcp_client = make_shared <TcpClient>();
	tcp_client->SetMessageFactory(shared_ptr <klicen::asio::ip::MessageFactory>(new ClientMessageFactory));
	tcp_client->Connect("127.0.0.1", 1234);
	thread t([]() { IoServicePool::singleton::GetInstance()->Run(); });
	t.detach();
	while (true)
	{
		cout << "please input send server string(enter to end): ";
		char buffer[1024];
		cin >> buffer;
		tcp_client->AsyncWrite(ToNetwork(buffer));
		this_thread::sleep_for(chrono::seconds(1));
		//cin.getline(buffer, size(buffer));
	}

	return EXIT_SUCCESS;
}