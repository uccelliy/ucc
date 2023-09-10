#include"log.h"
#include<stdarg.h>
namespace ucc{

	LogEvent::LogEvent(std::shared_ptr<Logger> Logger,LogLevel::Level level,const char* file, int32_t line , uint32_t elapse , uint32_t threadID , uint32_t fiberID , uint64_t time ):m_file(file),m_line(line),m_elapse(elapse),m_threadId(threadID),m_fiberId(fiberID),m_time(time),m_logger(Logger),m_level(level){} 


	
const char* LogLevel::Tostring(LogLevel::Level level){
    switch(level){
#define XX(name) \
	case LogLevel::name: \
	    return #name; \
	    break;
	XX(DEBUG);
	XX(INFO);
	XX(WARN);
	XX(ERROR);
	XX(FATAL);
#undef XX
	default:
	    return "UNKOWN";
    }
    return "UNKOWN";
}

LogEventWrap::LogEventWrap(LogEvent::ptr e):m_event(e){}
LogEventWrap::~LogEventWrap(){m_event->getlogger()->log(m_event->getlevel(),m_event);}
std::stringstream& LogEventWrap::getSS(){return m_event->getSS();}

void LogEvent::format(const char* fmt,...){
	va_list al;
	va_start (al,fmt);
	format(fmt,al);
	va_end(al);
}
void LogEvent::format(const char* fmt,va_list al){
	char* buf = nullptr;
	int len = vasprintf(&buf,fmt,al);
	if(len!=-1){
		m_ss<<std::string(buf,len);
		free(buf);
	}
}
class MessageFormatItem:public LogFormatter::FormatItem{
	public:
		MessageFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<event->getcontent();
		}
};

class LevelFormatItem:public LogFormatter::FormatItem{
	public:
		LevelFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<LogLevel::Tostring(level);
		}
};

class ElapseFormatItem:public LogFormatter::FormatItem{
	public:
		ElapseFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{

			os<<event->getelapse();
		}
};

class DateTimeFormatItem:public LogFormatter::FormatItem{
	public:
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			struct tm tm;
			time_t time=event->gettime();
			localtime_r(&time,&tm);
			char buf[64];
			strftime(buf,sizeof(buf),m_format.c_str(),&tm);
			os<<buf;}
		DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S"):m_format(format){if (m_format.empty()) m_format = "%Y-%m-%d %H:%M:%S";}
	private:
		std::string m_format;
};

class ThreadFormatItem:public LogFormatter::FormatItem{
	public:
		ThreadFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<event->getthreadId();
		}
};
class FiberFormatItem:public LogFormatter::FormatItem{
	public:
		FiberFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<event->getfiberId();
		}
};

class LineFormatItem:public LogFormatter::FormatItem{
	public:
		LineFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<event->getline();
		}
};
class NewLineFormatItem:public LogFormatter::FormatItem{
	public:
		NewLineFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<std::endl;
		}
};

class TabFormatItem:public LogFormatter::FormatItem{
	public:
		TabFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<'\t';
		}
};


class FileFormatItem:public LogFormatter::FormatItem{
	public:
		FileFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<event->getfile();;
		}
};

class NameFormatItem:public LogFormatter::FormatItem{
	public:
		NameFormatItem(const std::string& str = ""){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<Logger->getname();
		}
};

class StringFormatItem:public LogFormatter::FormatItem{
	public:
		StringFormatItem(const std::string& str):m_string(str){}
		void format(Logger::ptr Logger,std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override{
			os<<m_string;}
	private:
		std::string m_string;
		};


Logger::Logger(const std::string& name):m_name(name),m_level(LogLevel::DEBUG){
	m_formatter.reset(new LogFormatter("%d%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));

}

void Logger::log(LogLevel::Level level , const LogEvent::ptr event){
    if(level>=m_level){
	auto self = shared_from_this();
        for(auto& i:m_appenders){i->log(self,level,event);}
    }     
}
void Logger::debug(LogEvent::ptr event){
     log(LogLevel::DEBUG,event);
}
void Logger::info(LogEvent::ptr event){
     log(LogLevel::INFO,event);
}
void Logger::warn(LogEvent::ptr event){
     log(LogLevel::WARN,event);
}
void Logger::error(LogEvent::ptr event){
     log(LogLevel::ERROR,event);
}
void Logger::fatal(LogEvent::ptr event){
     log(LogLevel::FATAL,event);
}

void Logger::addAppender(LogAppender::ptr appender){
    if(!appender->getFormatter()){appender->setFormatter(m_formatter);}
	m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender){
    for(auto it = m_appenders.begin();it!= m_appenders.end();it++)
       {
        if(*it==appender) {m_appenders.erase(it);break;} 
       }
} 

FileLogAppender::FileLogAppender (const std::string& filename):m_filename(filename){}

void FileLogAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
    if(level>=m_level){
      m_filestream<<m_formatter->format(logger,level,event);
    }
}
void StdoutLogAppender::log(std::shared_ptr<Logger> logger , LogLevel::Level level,LogEvent::ptr event){
    if(level>=m_level){
      std::cout<< m_formatter->format(logger,level,event);
    }
}
bool FileLogAppender::reopen(){
    if(m_filestream){m_filestream.close();}
    m_filestream.open(m_filename);
    return !!m_filestream;
}

LogFormatter::LogFormatter(const std::string& pattern):m_pattern(pattern){init();}

std::string LogFormatter::format(Logger::ptr Logger,LogLevel::Level level,LogEvent::ptr event){
    std::stringstream ss;
    for(auto& i : m_items){
	i->format(Logger,ss,level,event);
    }
    return ss.str();
}

void LogFormatter::init(){
    //str format type
    std::vector<std::tuple<std::string , std::string , int>> vec;
    std::string nstr;
    for(size_t i =0;i<m_pattern.size();i++){
	if(m_pattern[i]!='%'){ nstr.append(1,m_pattern[i]); continue;}
	if((i+1)<m_pattern.size()){ if(m_pattern[i+1]=='%') nstr.append(1,'%');}
    size_t n = i+1;
	int fmt_status = 0;
	size_t fmt_begin = 0;
	std::string str;
	std::string fmt;
	while(n<m_pattern.size())
	{
	    if(!isalpha(m_pattern[n])&&m_pattern[n]!='{'&&m_pattern[n]!='}'){break;}
	    if(fmt_status == 0)
	    {
			if(m_pattern[n]=='{')
			{
				str = m_pattern.substr(i+1,n-i-1);
				fmt_status = 1;//jiexigeshi kaishi
				++n;
				fmt_begin = n;
				continue;
			}		
	    }
	    if(fmt_status==1)
	    {
			if(m_pattern[n]=='}')
			{
				fmt = m_pattern.substr(fmt_begin+1,n-fmt_begin-1);
				fmt_status = 2;
				break;		    
			}
	    }
		++n;
	}
	if(fmt_status == 0)
	{
	    if(!nstr.empty()){vec.push_back(std::make_tuple(nstr,"",0));nstr.clear();}
	    str = m_pattern.substr(i+1,n-i-1);
	    vec.push_back(std::make_tuple(str,fmt,1));
	    i = n-1;
	}
	else if(fmt_status==1)
	{
	    std::cout<<"pattern parse error: "<<m_pattern<<" - "<<m_pattern[i]<<std::endl;
	    vec.push_back(std::make_tuple("<partarn_error>",fmt,1));
	}
	else if(fmt_status==2)
	{
	    if(!nstr.empty()){vec.push_back(std::make_tuple(nstr,"",0));nstr.clear();}
	    vec.push_back(std::make_tuple(str,fmt,1));
	    i = n-1;
	}
    }
    if(!nstr.empty()) vec.push_back(std::make_tuple(nstr,"",0));

    static std::map<std::string,std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
	#define XX(str,C) \
		{#str,[](const std::string& fmt){return FormatItem::ptr (new C(fmt));}}

		XX(m,MessageFormatItem),
		XX(p,LevelFormatItem),
		XX(r,ElapseFormatItem),
		XX(c,NameFormatItem),
		XX(t,ThreadFormatItem),
		XX(l,LineFormatItem),
		XX(f,FileFormatItem),
		XX(d,DateTimeFormatItem),
		XX(n,NewLineFormatItem),
		XX(T,TabFormatItem),
		XX(F,FiberFormatItem),
	#undef XX
    };
 
    for(auto& i :vec){
	if(std::get<2>(i)==0){m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));}
	else{
		auto it =s_format_items.find(std::get<0>(i)); 
		if(it == s_format_items.end()){m_items.push_back(FormatItem::ptr(new StringFormatItem("<error  %"+std::get<0>(i) + ">")));}
		else{m_items.push_back(it->second(std::get<1>(i)));}			
	    }

	//std::cout <<"(" <<std::get<0>(i) << ")-("<<std::get<1>(i)<<")-("<< std::get<2>(i)<<")"<<std::endl;
    } 
	//std::cout << m_items.size()<<std::endl;
}
 
LogManager::LogManager(){
	m_root.reset(new Logger);
	m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));
}
Logger::ptr LogManager::getLogger(const std::string& name){
	auto it = m_loggers.find(name);
	return it == m_loggers.end()?m_root : it->second;
}

}

