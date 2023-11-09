#ifndef __UCC_IOMANAGER_H__
#define __UCC_IOMANAGER_H__

#include"scheduler.h"
#include"timer.h"

namespace ucc{

class IOManager : public Scheduler ,public TimerManager{
public:
	typedef std::shared_ptr<IOManager> ptr;
	typedef RWMutex RWMutexType;
	
	enum Event {
		NONE    = 0x0,
		READ    = 0x1,
		WRITE   = 0x4,
	};
private:
	struct FdContext{
		typedef Mutex MutexType;
		struct EventContext{
			Scheduler* scheduler = nullptr;
			Fiber::ptr fiber;
			std::function<void()> cb;
		};

		EventContext& getContext(Event event);
		void resetContext(EventContext& ctx);
		void triggerEvent(Event event);

		Event events = NONE;
		int fd = 0;
		EventContext read;
		EventContext write;
		MutexType mutex;
	};

public:
	IOManager(size_t threads = 1,bool use_caller = true , const std::string& name = "");
	~IOManager();

	//1 success 0 retry -1 error
	int addEvent(int fd , Event event, std::function<void()> cb = nullptr);
	bool delEvent(int fd,Event event);
	bool cancelEvent(int fd , Event event);

	bool cancelAll(int fd);

	static IOManager* GetThis();

protected:
	void tickle() override;
	bool stopping() override;
	bool stopping(uint64_t& timeout);
	void idle() override;
	void onTimerInsertedAtFront() override;

	void contextResize(size_t size);
private:
	int m_epfd = 0;
	int m_tickleFds[2];

	std::atomic<size_t> m_pendinfEventCount= {0};
	RWMutexType m_mutex;
	std::vector<FdContext*> m_fdContexts;
};

}



#endif
