#include"../ucc/ucc.h"
#include<unistd.h>

ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

int count = 0;
//ucc::RWMutex s_mutex;
ucc::Mutex s_mutex;
void fun1(){
	UCC_LOG_INFO(g_logger)<<"name = "<<ucc::Thread::GetName()
						  <<"this.name = "<<ucc::Thread::GetThis()->getName()
						  <<"id = "<<ucc::GetThreadId()
						  <<"this.id = "<<ucc::Thread::GetThis()->getId();
	for(int i = 0;i<10000000;i++){
		ucc::Mutex::Lock lock(s_mutex);
		++count;
	}
}

void fun2(){
	while(true){
		UCC_LOG_INFO(g_logger)<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	}	
}

void fun3(){
	while(true){
		UCC_LOG_INFO(g_logger)<<"=====================================================";
	}
}

int main(int argc , char** argv){
	UCC_LOG_INFO(g_logger)<<"thread test begin";
	YAML::Node root = YAML::LoadFile("/home/yinruochen/workspace/ucc/bin/conf/log2.yml");
	ucc::Config::LoadFromYaml(root);
	std::vector<ucc::Thread::ptr> thrs;
	for(int i = 0 ;i<2;i++){
		ucc::Thread::ptr thr(new ucc::Thread(&fun2,"name_"+ std::to_string(i*2)));
		ucc::Thread::ptr thr2(new ucc::Thread(&fun3,"name_"+ std::to_string(i*2+1)));
		thrs.push_back(thr);
		thrs.push_back(thr2);
	}
	for(size_t i = 0; i<thrs.size();++i){
		thrs[i]->join();
	}
	UCC_LOG_INFO(g_logger)<<"thread test end";
	UCC_LOG_INFO(g_logger)<<"count= "<<count;

	return 0;
}
