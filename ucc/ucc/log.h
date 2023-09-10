#ifndef __UCC_LOG_H__
#define __UCC_LOG_H__
#include<string>
#include<stdint.h>
#include<memory>
#include<list>
#include<sstream>
#include<fstream>
#include<map>
#include<vector>
#include<utility>
#include<iostream>
#include<functional>
#include<time.h>
#include<string.h>
#include"singleton.h"
#include"util.h"
#define UCC_LOG_LEVEL(logger,level) \
	if(logger->getLevel()<=level) \
		ucc::LogEventWrap(ucc::LogEvent::ptr(new ucc::LogEvent(logger,level,__FILE__,__LINE__,0,ucc::GetThreadId(),ucc::GetFiberId(),time(0)))).getSS()
#define UCC_LOG_DEBUG(logger) UCC_LOG_LEVEL(logger,ucc::LogLevel::DEBUG)
#define UCC_LOG_INFO(logger) UCC_LOG_LEVEL(logger,ucc::LogLevel::INFO)
#define UCC_LOG_WARN(logger) UCC_LOG_LEVEL(logger,ucc::LogLevel::WARN)
#define UCC_LOG_ERROR(logger) UCC_LOG_LEVEL(logger,ucc::LogLevel::ERROR)
#define UCC_LOG_FATAL(logger) UCC_LOG_LEVEL(logger,ucc::LogLevel::FATAL)

#define UCC_LOG_FMT_LEVEL(logger,level,fmt,...) \
	if(logger->getLevel()<= level) \
		ucc::LogEventWrap(ucc::LogEvent::ptr(new ucc::LogEvent(logger,level,__FILE__,__LINE__,0,ucc::GetThreadId(),ucc::GetFiberId(),time(0)))).getEvent()->format(fmt, __VA_ARGS__)
#define UCC_LOG_FMT_DEBUG(logger,fmt,...) UCC_LOG_FMT_LEVEL(logger,ucc::LogLevel::DEBUG,fmt,__VA_ARGS__)
#define UCC_LOG_FMT_INFO(logger,fmt,...) UCC_LOG_FMT_LEVEL(logger,ucc::LogLevel::INFO,fmt,__VA_ARGS__)
#define UCC_LOG_FMT_WARN(logger,fmt,...) UCC_LOG_FMT_LEVEL(logger,ucc::LogLevel::WARN,fmt,__VA_ARGS__)
#define UCC_LOG_FMT_ERROR(logger,fmt,...) UCC_LOG_FMT_LEVEL(logger,ucc::LogLevel::ERROR,fmt,__VA_ARGS__)
#define UCC_LOG_FMT_FATAL(logger,fmt,...) UCC_LOG_FMT_LEVEL(logger,ucc::LogLevel::FATAL,fmt,__VA_ARGS__)

#define UCC_LOG_ROOT() ucc::LogMgr::GetInstance()->getRoot()

namespace ucc {
class LogLevel{
public:
    enum Level{
  	UNKOWN = 0,
	DEBUG = 1,
	INFO = 2,
	WARN = 3,
	ERROR = 4,
	FATAL = 5,
    };
    static const char* Tostring(LogLevel::Level level);	
};


class Logger;
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> Logger,LogLevel::Level level,const char* file, int32_t line , uint32_t elapse , uint32_t threadID , uint32_t fiberId , uint64_t time);
    const char* getfile() const{return m_file;}
    int32_t getline() const{return m_line;}
    uint32_t getelapse() const{return m_elapse;}
    uint32_t getthreadId() const{return m_threadId;}
    uint32_t getfiberId() const{return m_fiberId;}
    uint64_t gettime() const{return m_time;}
    std::string getcontent() const{return m_ss.str();}
	std::shared_ptr<Logger> getlogger() const{return m_logger;}
	LogLevel::Level getlevel()const{return m_level;}
	std::stringstream& getSS(){return m_ss;}
	void format(const char* fmt,...);
	void format(const char* fmt, va_list al);
private:
    const char* m_file = nullptr;//filename
    int32_t m_line = 0;//line number
    uint32_t m_elapse = 0;//running time
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time=0;
    std::stringstream m_ss;
	std::shared_ptr<Logger> m_logger;
	LogLevel::Level m_level;
	};
class LogEventWrap{
public:
	LogEventWrap(LogEvent::ptr e);
	~LogEventWrap();
	std::stringstream& getSS();
	LogEvent::ptr getEvent()const{return m_event;}
private:
	LogEvent::ptr m_event;
};
//formatter
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern); 
    std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);  
public:
    class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual  ~FormatItem(){};
		virtual void format( std::shared_ptr<Logger> logger , std::ostream& os,LogLevel::Level level, LogEvent::ptr event)=0;
    };
    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;     
};

//log output destination appender
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){} 	
    virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr var){m_formatter = var;}  
    LogFormatter::ptr getFormatter()const{return m_formatter;}
	LogLevel::Level getlevel()const{return m_level;}
	void setlevel(LogLevel::Level level){m_level = level;}
protected:
    LogLevel::Level m_level=LogLevel::DEBUG;
    LogFormatter::ptr m_formatter;
};

//log ouput logger
class Logger: public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name="root");
    void log(LogLevel::Level level,const LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel()const{return m_level;}
    void setLevel(LogLevel::Level val){m_level = val;}
    const std::string getname()const{return m_name;}
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders;
	LogFormatter::ptr m_formatter;
};


//output to cmd
class StdoutLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override;
private:
};
//output to file
class FileLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override;
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};
class LogManager{
public:
	LogManager();
	Logger::ptr getLogger(const std::string& name);
	void init();
	Logger::ptr getRoot()const{return m_root;}
private:
	std::map<std::string,Logger::ptr> m_loggers;
	Logger::ptr m_root;


};

typedef ucc::Singleton<LogManager> LogMgr;

}


#endif
