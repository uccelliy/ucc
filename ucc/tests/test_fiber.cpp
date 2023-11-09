#include"../ucc/ucc.h"

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();
void run_in_fiber(){
	UCC_LOG_INFO(g_logger) << "run in fiber begin ";
	ucc::Fiber::YieldToHold();
	UCC_LOG_INFO(g_logger) << "run in fiber end";
	ucc::Fiber::YieldToHold();
}

void test_fiber(){
	UCC_LOG_INFO(g_logger)<<"main begin -1";
	{
		ucc::Fiber::GetThis();
		UCC_LOG_INFO(g_logger) <<"main begin";
		ucc::Fiber::ptr fiber(new ucc::Fiber(run_in_fiber));
		fiber->swapIn();
		UCC_LOG_INFO(g_logger)<<"main after swapIn";
		fiber->swapIn();
		UCC_LOG_INFO(g_logger)<<"main after end";
		fiber->swapIn();
	}
	UCC_LOG_INFO(g_logger)<<"main begin -1";
}

int main(int argc,char** argv){
	ucc::Thread::SetName("main");
	
	std::vector<ucc::Thread::ptr> thrs;
	for(int i = 0;i<3;i++){
		thrs.push_back(ucc::Thread::ptr(
				new ucc::Thread(&test_fiber,"name"+ std::to_string(i))));
	}

	for(auto i:thrs){
		i->join();
	}
	return 0;
}
