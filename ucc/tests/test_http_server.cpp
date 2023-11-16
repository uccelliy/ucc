#include"../ucc/http/http_server.h"
#include"../ucc/log.h"

static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void run(){
	ucc::http::HttpServer::ptr server(new ucc::http::HttpServer);
	ucc::Address::ptr addr = ucc::Address::LookupAnyIPAddress("0.0.0.0:8020");
	while(!server->bind(addr)){
		sleep(2);
	}
	auto sd = server->getServletDispatch();
	sd->addServlet("/ucc/xx", [](ucc::http::HttpRequest::ptr req
							,ucc::http::HttpResponse::ptr rsp
							,ucc::http::HttpSession::ptr session){
				rsp->setBody(req->toString());
				return 0;
			});

	sd->addServlet("/ucc/*", [](ucc::http::HttpRequest::ptr req
							,ucc::http::HttpResponse::ptr rsp
							,ucc::http::HttpSession::ptr session){
				rsp->setBody("Glob:\r\n"+req->toString());
				return 0;
			});

	server->start();
}

int main(int argc , char** argv){
	ucc::IOManager iom(2);
	iom.schedule(run);
	return 0;

}
