#include"../ucc/tcp_server.h"
#include"../ucc/log.h"
#include"../ucc/iomanager.h"
#include"../ucc/bytearray.h"
#include"../ucc/address.h"

static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

class EchoServer :public ucc::TcpServer{
public:
	EchoServer(int type);
	void handleClient(ucc::Socket::ptr client);
private:
	int m_type = 0;
};

EchoServer::EchoServer(int type)
	:m_type(type)
	{
}
	
void EchoServer::handleClient(ucc::Socket::ptr client){
	UCC_LOG_INFO(g_logger)<<"handleClient"<<*client;
	ucc::ByteArray::ptr ba(new ucc::ByteArray);
	while(true){
		ba->clear();
		std::vector<iovec> iovs;
		ba->getWriteBuffers(iovs,1024);

		int rt = client->recv(&iovs[0] , iovs.size());
		if(rt == 0){
			UCC_LOG_INFO(g_logger)<<"client close:"<<*client;
			break;
		}else if(rt<0){
			UCC_LOG_INFO(g_logger)<<"client error rt = "<<rt
				<<"errno = "<<errno<<"errstr = "<<strerror(errno);
			break;
		}
		ba->setPosition(ba->getPosition() + rt);

		ba->setPosition(0);
		if(m_type == 1){
			UCC_LOG_INFO(g_logger)<<ba->toString();
		}else{
			UCC_LOG_INFO(g_logger)<<ba->toHexString();
		}
	}
}
int type = 1;

void run(){
	EchoServer::ptr es(new EchoServer(type));
	auto addr = ucc::Address::LookupAny("0.0.0.0:8020");
	while(!es->bind(addr)){
		sleep(2);
	}
	es->start();
}


int main(int argc , char** argv){
	if(argc<2){
		UCC_LOG_INFO(g_logger)<<"used as["<<argv[0]<<"-t] or ["<<argv[0]<<"-b]";
		return 0;
	}

	if(!strcmp(argv[1],"-b")){
		type = 2;
	}
	ucc::IOManager iom(2);
	iom.schedule(run);
	return 0;
}
