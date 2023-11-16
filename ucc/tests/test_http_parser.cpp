#include"../ucc/http/http_parser.h"
#include"../ucc/log.h"


static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

const char test_request_data[] = "POST / HTTP/1.1\r\n"
	"Host: www.sylar.top\r\n"
	"Content-Length: 10\r\n"
	"1234567890";

void test_request(){
	
	ucc::http::HttpRequestParser parser;
	std::string tmp = test_request_data;
	size_t s = parser.execute(&tmp[0], tmp.size());
	UCC_LOG_INFO(g_logger)<<"excute rt = "<<s
		<<"has_error="<<parser.hasError()
		<<"is_finished = "<<parser.isFinished()
		<<"total"<<tmp.size()
		<<"content_length="<<parser.getContentLength();
	tmp.resize(tmp.size()-s);

	UCC_LOG_INFO(g_logger)<<parser.getData()->toString();
}

const char test_response_data[] = "HTTP/1.0 200 OK\r\n";



//void test_response(){
//	ucc::http::HttpResponseParser::ptr parser;
//	std::string tmp = test_response_data;
//	size_t s = parser.execute(&tmp[0] , tmp.size());
//}

int main (int argv ,char** argc){
	//test_request();
	return 0;
}
