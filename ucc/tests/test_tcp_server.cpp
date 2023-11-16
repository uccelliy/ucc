#include "../ucc/tcp_server.h"
#include "../ucc/iomanager.h"
#include "../ucc/ucc.h"


ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void run(){
	auto addr = ucc::Address::LookupAny("0.0.0.0:8033");
	//auto addr2 = ucc::UnixAddress::ptr(new ucc::UnixAddress("/tmp/unix_addr"));
	std::vector<ucc::Address::ptr> addrs;
	addrs.push_back(addr);
	//addrs.push_back(addr2);

	ucc::TcpServer::ptr tcp_server(new ucc::TcpServer);
	std::vector<ucc::Address::ptr> fails;

	while(!tcp_server->bind(addrs,fails)){
		sleep(2);
	}
	tcp_server->start();
}


int main(int argc , char** argv){
	ucc::IOManager iom(2);
	iom.schedule(run);

	return 0;
}
