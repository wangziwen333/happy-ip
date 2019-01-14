#include <ip/server/udp_server.h>
#include "proto_server_message_factory.h"
#ifdef GLOG_OUTPUT
#include <config_glog.h>
#endif
using namespace happy::asio::ip;

int main(int argc, char* argv[])
{
	// 默认创建线程池的大小为4。
	//IoServicePool::singleton::Create(4); 
#ifdef GLOG_OUTPUT
	utils::ConfigGlog(argv[0]);
#endif
	uint16_t port = 1235;
	UdpServer udp_server(port);
	printf("udp server is run (port=%d)\n", port);
	udp_server.SetMessageFactory(make_shared <ProtoServerMessageFactory>("", true));
	IoServicePool::singleton::GetInstance()->Run();

	return EXIT_SUCCESS;
}