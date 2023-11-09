#include"../ucc/ucc.h"
#include<assert.h>

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void test_assert(){
	UCC_LOG_INFO(g_logger)<<ucc::BacktraceToString(10);
	UCC_ASSERT2(false,"12234");
}

int main(int argc,char** argv){
	test_assert();
	return 0;
}
