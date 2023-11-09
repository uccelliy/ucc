#include"../ucc/ucc.h"

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void test_fiber(){
	UCC_LOG_INFO(g_logger)<<"test in fiber";

	static int s_count = 5;
	sleep(1);
	if(--s_count >= 0){
	ucc::Scheduler::GetThis()->schedule(&test_fiber);
	}
}


int main(int argc , char** argv){
	UCC_LOG_INFO(g_logger)<<"main";
	ucc::Scheduler sc(3,false,"test");
	sc.start();
	UCC_LOG_INFO(g_logger)<<"start over";
	sc.schedule(&test_fiber);
	sc.stop();
	UCC_LOG_INFO(g_logger)<<"over" + std::to_string(1);
	return 0;
}
