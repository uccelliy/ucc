#include<iostream>
#include"../ucc/log.h"
#include"../ucc/util.h"

int main(int argc,char** argv){
	ucc::Logger::ptr logger(new ucc::Logger);
	logger->addAppender(ucc::LogAppender::ptr (new ucc::StdoutLogAppender));
	ucc::FileLogAppender::ptr file_appender(new ucc::FileLogAppender("./log.txt"));
	file_appender->reopen();
	ucc::LogFormatter::ptr fmt(new ucc::LogFormatter("%d%T%p%T&m%n"));
	file_appender->setFormatter(fmt);
	file_appender->setlevel(ucc::LogLevel::ERROR);
	logger->addAppender(file_appender);
	//ucc::LogEvent::ptr event(new ucc::LogEvent(__FILE__,__LINE__,0,ucc::GetThreadId(),ucc::GetFiberId(),time(0)));
	//event->getSS() << "hello ucc log";
	//logger->log(ucc::LogLevel::DEBUG,event);
	std::cout <<"hello ucc log" <<std::endl;

	UCC_LOG_INFO(logger)<< "test macro";
	UCC_LOG_ERROR(logger)<<"test macro error";
	UCC_LOG_FMT_ERROR(logger,"test macro fmt error %s","aa");
	auto l = ucc::LogMgr::GetInstance()->getLogger("xx");
	UCC_LOG_INFO(l)<<"xxx";
    return 0;
}
