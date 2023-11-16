#ifndef __UCC_HTTP_HTTP_SERVER_H__
#define __UCC_HTTP_HTTP_SERVER_H__

#include"../tcp_server.h"
#include"http_session.h"
#include"servlet.h"

namespace ucc{
namespace http{

class HttpServer : public TcpServer{
public:
	typedef std::shared_ptr<HttpServer> ptr;
	HttpServer(bool keepalive = false,
				ucc::IOManager* worker = ucc::IOManager::GetThis()
				,ucc::IOManager* accept_worker = ucc::IOManager::GetThis());
	
	ServletDispatch::ptr getServletDispatch() const {return m_dispatch;}
	void setServletDispatch(ServletDispatch::ptr v){m_dispatch = v;}
protected:
	virtual void handleClient(Socket::ptr client) override;
private:
	bool m_isKeepalive;
	ServletDispatch::ptr m_dispatch;
};


}
}



#endif
