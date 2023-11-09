#include"../ucc/socket.h"
#include"../ucc/ucc.h"
#include"../ucc/iomanager.h"
#include"../ucc/address.h"

static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void test_socket(){
	ucc::IPAddress::ptr addr = ucc::Address::LookupAnyIPAddress("www.baidu.com");
	if(addr){
		UCC_LOG_INFO(g_logger)<<"get address:"<<addr->toString();
	}else{
		UCC_LOG_ERROR(g_logger)<<"get address fail";
		return;
	}

	ucc::Socket::ptr sock = ucc::Socket::CreateTCP(addr);
	addr->setPort(80);
	UCC_LOG_INFO(g_logger)<<"addr="<<addr->toString();
	if(!sock->connect(addr)){
		UCC_LOG_ERROR(g_logger)<<"connect"<<addr->toString()<<"fail";
		return;
	}else{
		UCC_LOG_INFO(g_logger)<<"connect"<<addr->toString()<<"connected";
	}

	const char buff[] = "GET / HTTP/1.0\r\n\r\n";
	int rt = sock->send(buff , sizeof(buff));	
	if(rt <=0 ){
		UCC_LOG_INFO(g_logger)<<"send fail rt="<<rt;
		return;
	}

	std::string buffs;
	buffs.resize(4096);
	rt = sock->recv(&buffs[0],buffs.size());

	if(rt<=0){
		UCC_LOG_INFO(g_logger)<<"send fail rt="<<rt;
		return ;
	}

	buffs.resize(rt);
	UCC_LOG_INFO(g_logger)<<buffs;
}

int main(int argc , char** argv){
	ucc::IOManager iom;
	iom.schedule(&test_socket);
	return 0;
}
