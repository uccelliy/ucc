#include"../ucc/ucc.h"
#include"../ucc/iomanager.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();
int sock = 0;

void test_fiber(){
	UCC_LOG_INFO(g_logger)<<"test_fiber sock ="<<sock;
	sock = socket(AF_INET , SOCK_STREAM , 0);
	fcntl(sock , F_SETFL , O_NONBLOCK);

	sockaddr_in addr;
	memset(&addr , 0 , sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_pton(AF_INET , "220.181.38.149",&addr.sin_addr.s_addr);
	
	if(!connect(sock,(const sockaddr*)&addr,sizeof(addr))){
	}else if(errno == EINPROGRESS){
		UCC_LOG_INFO(g_logger)<<"add event errno="<<errno<<""<<strerror(errno);
		ucc::IOManager::GetThis()->addEvent(sock,ucc::IOManager::READ ,[](){
			UCC_LOG_INFO(g_logger)<<"read cb";
			});
		ucc::IOManager::GetThis()->addEvent(sock , ucc::IOManager::WRITE, [](){
			UCC_LOG_INFO(g_logger)<<"write cb";
			ucc::IOManager::GetThis()->cancelEvent(sock ,ucc::IOManager::READ);
			close(sock);
			});
	}else{
		UCC_LOG_INFO(g_logger)<<"else"<<errno<<""<<strerror(errno);
	}	
}

void test1(){
	std::cout<<"EPOLLIN="<<EPOLLIN
			<<"EPOLLOUT="<<EPOLLOUT<<std::endl;
	ucc::IOManager iom(2,false);
	iom.schedule(&test_fiber);
}

ucc::Timer::ptr s_timer;
void test_timer(){
	ucc::IOManager iom(2);
	s_timer = iom.addTimer(1000,[](){
			UCC_LOG_INFO(g_logger)<<"hello timer";
			static int i = 0;
			if(++i == 5){
				s_timer->reset(2000,true);
			}
		},true);
}

int main(int argc, char** argv){
	//test1();
	test_timer();
	return 0 ;
}
