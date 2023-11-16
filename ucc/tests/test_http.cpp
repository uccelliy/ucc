#include"../ucc/http/http.h"
#include"../ucc/log.h"
void test(){
	ucc::http::HttpRequest::ptr req(new ucc::http::HttpRequest());
	req->setHeader("host" , "www.sylar.top");
	req->setBody("hello sylar");

	req->dump(std::cout)<<std::endl;
}

void test_response(){
	ucc::http::HttpResponse::ptr rsp(new ucc::http::HttpResponse);
	rsp->setHeader("X-X" , "sylar");
	rsp->setBody("hello sylar");
	rsp->setStatus((ucc::http::HttpStatus)400);
	rsp->setClose(false);
	rsp->dump(std::cout)<<std::endl;
}

int main(int argc , char** argv){
	test();
	test_response();
	return 0;
}
