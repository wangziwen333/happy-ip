#include <ip/server/tcp_server.h>
#include "server_message_factory.h"
using namespace klicen::asio::ip;

int main()
{
	// 默认创建线程池的大小为4。
	//IoServicePool::singleton::Create(4); 
	uint16_t port = 1234;
	TcpServer tcp_server(port);
	printf("tcp server is run (port=%d)\n", port);
	tcp_server.SetMessageFactory(make_shared <ServerMessageFactory>());
	IoServicePool::singleton::GetInstance()->Run();

	return EXIT_SUCCESS;
}