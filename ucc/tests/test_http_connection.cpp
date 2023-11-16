#include"../ucc/http/http_connection.h"
#include<iostream>
#include"../ucc/log.h"
#include"../ucc/iomanager.h"

static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();


void run(){
	ucc::Address::ptr addr = ucc::Address::LookupAnyIPAddress("www.sylar.top:80");
	if(!addr){
		UCC_LOG_INFO(g_logger)<<"get addr error";
		return;
	}
	
	ucc::Socket::ptr sock = ucc::Socket::CreateTCP(addr);
	bool rt = sock->connect(addr);
	if(!rt){
		UCC_LOG_INFO(g_logger)<<"connect "<<*addr<<" failed";
		return;
	}

	ucc::http::HttpConnection::ptr conn(new ucc::http::HttpConnection(sock));
	ucc::http::HttpRequest::ptr req(new ucc::http::HttpRequest);
	req->setPath("/blog/");
	req->setHeader("host" , "www.sylar.top");
	UCC_LOG_INFO(g_logger)<<"req:"<<std::endl
		<<*req;

	conn->sendRequest(req);
	auto rsp = conn->recvResponse();

	if(!rsp){
		UCC_LOG_INFO(g_logger)<<"recv response error";
		return;
	}
	UCC_LOG_INFO(g_logger)<<"rsp:"<<std::endl
		<<*rsp;
	
}

int main(int argc , char** argv){
	ucc::IOManager iom(2);
	iom.schedule(run);
	return 0;
		
}
