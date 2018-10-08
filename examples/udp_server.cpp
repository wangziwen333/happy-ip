#include <ip/server/udp_server.h>
#include "server_message_factory.h"
using namespace happy::asio::ip;

int main()
{
	// 默认创建线程池的大小为4。
	//IoServicePool::singleton::Create(4); 
	uint16_t port = 1235;
	UdpServer udp_server(port);
	printf("udp server is run (port=%d)\n", port);
	udp_server.SetMessageFactory(make_shared <ServerMessageFactory>());
	IoServicePool::singleton::GetInstance()->Run();

	return EXIT_SUCCESS;
}