#include"../ucc/address.h"
#include"../ucc/log.h"

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void test(){
	std::vector<ucc::Address::ptr> addrs;

	bool v = ucc::Address::Lookup(addrs , "www.baidu.com");
	if(!v){
		UCC_LOG_ERROR(g_logger)<<"lookup fail";
		return;
	}

	for(size_t i = 0; i < addrs.size() ; ++i){
		UCC_LOG_INFO(g_logger)<<i<<"-"<<addrs[i]->toString();
	}
}

void test_iface(){
	std::multimap<std::string , std::pair<ucc::Address::ptr , uint32_t> > results;

	bool v = ucc::Address::GetInterfaceAddress(results);
	if(!v){
		UCC_LOG_ERROR(g_logger)<<"GetInterfaceAddress fail";
		return;
	}

	for(auto& i :results){
		UCC_LOG_INFO(g_logger)<<i.first <<"-"<<i.second.first->toString()<<"- "
			<<i.second.second;
	}
}

void test_ipv4(){
	auto addr = ucc::IPAddress::Create("127.0.0.8");
	if(addr){
		UCC_LOG_INFO(g_logger)<<addr->toString();
	}

}

int main (int argc , char** argv){
	//test();
	//test_iface();
	test_ipv4();
	return 0;
}
