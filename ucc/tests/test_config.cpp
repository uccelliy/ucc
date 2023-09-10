#include "../ucc/config.h"
#include "../ucc/log.h"
ucc::ConfigVar<int>::ptr g_int_value_config = 
	ucc::Config::Lookup("systrm.port", (int)8080, "system port");

ucc::ConfigVar<float>::ptr g_float_value_config = 
	ucc::Config::Lookup("systrm.value", (float)10.2f, "system value");




int main(int argc,char** argv){
	UCC_LOG_INFO(UCC_LOG_ROOT())<<g_int_value_config->getValue();
	UCC_LOG_INFO(UCC_LOG_ROOT())<<g_float_value_config->toString();
	return 0;
	
}
