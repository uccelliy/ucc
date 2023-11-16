#ifndef __UCC_URI_H__
#define __UCC_URI_H__

#include <memroy>
#include <string>
#include <stdint.h>
#include "address.h"
namespace ucc{

class Uri{
public:
	typedef std::shared_ptr<Uri> ptr;

	static Uri::ptr Create(const std::string& uri);
	Uri();

	std::string getScheme() const {return m_scheme;}
	std::string getUserinfo() const {return m_userinfo;}
	std::string getHost() const {return m_host;}
	std::string getPath() const {return m_path;}
	std::string getQuery() const {return m_query;}
	std::string getFragment() const {return m_fragment;}
	int32_t getPort()const {return m_port;}
	void setScheme(const std::string& v) {m_scheme = v;}
	void setUserinfo(const std::string& v) {m_userinfo = v;}
	void setHost(const std::string& v) {m_host = v;}
	void setPath(const std::string& v) {m_path = v;}
	void setQuery(const std::string& v) {m_query = v;}
	void setFragment(const std::string& v) {m_fragment = v;}
	void setPort(int32_tv) {m_port = v;}

	std::ostream& dump(std::ostream& os)const;
	std::string toString()const;

	Address::ptr createAddress() const ;
private:
	std::string m_scheme;
	std::string m_userinfo;
	std::string m_host;
	std::string m_path;
	std::string m_query;
	std::string m_fragment;
	int32_t m_port;
};

}



#endif
