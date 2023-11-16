#ifndef __UCC_HTTP_HTTP_PARSER_H__
#define __UCC_HTTP_HTTP_PARSER_H__

#include"http.h"
#include"http11_parser.h"
#include"httpclient_parser.h"

namespace ucc{
namespace http{
class HttpRequestParser{
public:
	typedef std::shared_ptr<HttpRequestParser> ptr;
	HttpRequestParser();

	size_t execute(char* data , size_t len );
	int isFinished() ;
	int hasError() ;

	HttpRequest::ptr getData() const {return m_data;}
	void setError(bool v){m_error = v;}
	
	uint64_t getContentLength();
	const http_parser& getParser()const {return m_parser;}
public:
static uint64_t GetHttpRequestBufferSize();
static uint64_t GetHttpRequestMaxBodySize();
private:
	http_parser m_parser;
	HttpRequest::ptr m_data;
	int m_error;
};


class HttpResponseParser{
public:
	typedef std::shared_ptr<HttpResponseParser> ptr;
	HttpResponseParser();

	size_t execute( char* data , size_t len , bool chunk);
	int isFinished() ;
	int hasError() ;
	
	HttpResponse::ptr getData() const {return m_data;}
	void setError(bool v){m_error = v;}
	
	uint64_t getContentLength();
	const httpclient_parser& getParser()const {return m_parser;}
public:
static uint64_t GetHttpResponseBufferSize();
static uint64_t GetHttpResponseMaxBodySize();

private:
	httpclient_parser m_parser;
	HttpResponse::ptr m_data;
	int m_error;
};



}
}

#endif
