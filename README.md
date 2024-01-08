# ucc
study project，一个服务器框架现在只实现了静态服务器。

## 日志模块（log4j）
日志模块在log.h中定义生成日志并输出的类，在log.cpp中实现，最关键的实现是log.cpp的格式初始化方法，该方法接受用户自主输入的日志输出选项，解析并按照用户需求输出日志。此外就是日志输出器的两个继承类，一个负责输出到控制台，另外一个负责输出到文件。
此外比较困难的在于使用了大量的宏来实现众多重复的函数，比如各类日志格式函数，日志生产函数。

## 线程/协程模块与线程池
线程模块基于pthread.h,semaphore.h,atomic实现，
在thread.h中定义了信号量（sem_init,sem_destory,sem_wait,sem_post），互斥量，自旋锁，读写锁（读写互斥量），与线程类，并进一步把锁进行封装，使用时无需考虑不同锁内部的不同操作。
其中创建线程pthread_create，线程间通信（pthread_join），线程释放（pthread_detach）的方法进行封装。
在创建线程时传入自己定义的run函数，在run函数中真正执行任务，任务是由std::function定义的（函数指针），在调用构造函数时需传入要执行的任务，通过swap把任务放到内存中，然后执行。

协程模块基于ucontext.h实现，
在fiber.h中实现了fiber类。定义的协程有六个状态，INIT(初始化中),HOLD(阻塞中),EXEC(执行中),TERM(执行完毕),READY(无任务但准备就绪),EXCEPT(异常)。实际上任务是在协程中执行的，协程通过上面提到的run函数传入thread中，thread可以切换执行的协程。
fiber中定义的函数
reset()把一个处于初始化/异常/终止的协程重新初始化
swapIn()现在线程池中的主协程开始执行
swapOut()把现在这个协程停掉，换成协程池中的协程
call()现在这个协程开始执行
back()
以下是静态方法（只能处理静态变量），类中定义的函数（在类中就实现的了的）都会自动成为内联函数，在编译时就自动替换。
SetThis(Fiber* f)把一协程变为主协程
GetThis() 获得主协程的指针，如果当前主协程是空的就创建一个新的协程返回出去。
YieldToReady() 把主协程改为READY并调用swapOut
YieldToHold() 改为Hold
TotalFibers()
MainFunc() 执行函数，把cb里面的函数指针指向的函数执行了，如果有异常并把协程的状态设为异常
CallMainFunc()
GetFiberId()
在具体实现中，首先是定义两个静态全局线程拥有的t_fiber作为主协程，因为ucontext.h的关键在于getcontext，setcontext，makecontext，swapcontext三个函数

线程池模块
线程池模块在scheduler部分中实现，主要为scheduler类。类中有scheduler的智能指针和互斥锁。构造函数需要三个形参分别是初始线程数默认为1，use_caller默认为true，和一个字符串作为线程池的名字。线程池中用一个线程数组存储线程，把协程存放在一个FiberAndThread的列表里。还有一个根协程。一个主线程池（线程静态），一个主协程（线程静态）。采用轮询，每个fiber代表一个任务，或者直接传入需调用的方法。如果没用在执行就拿出执行，执行一段时间放出控制权，继续轮询，直到idle函数执行完都没有新任务就注销掉。
start()
stop()在线程数为0，根协程存在并且是终止或者初始化状态才能终止，如果根线程存在那么当前的线程池是应当是当前的线程负责的线程池，把线程池中的线程都放到一个新的数组中，再把数组中的线程join关闭掉。
模板函数schedule(T,int)
tickle()
run()运行函数
stoppoing()
idle()
模板函数scheduleNoLock(T,int)
结构体FiberAndThread 其中包括构造函数和一个方法reset()，属性有上面定义的协程的智能指针，一个函数指针，int。

## IOManager

## hook与异步IO

## 网络编程相关
IP地址
把IPv4，IPv6，Unix地址封装到一起。

序列化
序列化中进行序列化与反序列化，方便网络传输。序列化底层都是使用read和write方法实现


字节序
检测字节序，提供了两个函数分别按照大端序和小端序输出输入的序列。

socket相关
socket.h中封装了socket的相关函数（socket，connect，listen..）
socket_stream中封装了io相关函数

## 定时器
定时器头文件中实现TimeManager和Timer两个类，他们互为友元，定时器的抽象为Timer而TimerManager是Timer的管理类，也是外部接口。在Timer中有三种行为分别为取消，更新和重置。以及声明了一个仿函数，作为比较器（在后面实现最小堆时作为自定义的比较函数）。在TimerManager中有一个存放Timer的集合（最小堆）。获得时间的方式是调用了linux函数gettimeofday()函数。Timer中如果到了计时时间就会上锁并且执行其中的回调函数。TimeManager中的一个重要函数是listExpiredCb这个函数会对已经过期的定时器进行检查，如果这个定时器指定了recur那么就把定时器的时间更新再次放入执行。否则就把定时器中的回调函数置为空。

## 句柄管理器
句柄管理器与定时器相似，有FdCtx和FdManager两个类，FdCtx负责管理句柄，而FdManager相当于是FdCtx的封装。FdCtx里面的方法包括
构造函数中传入一个句柄，把标志位都设为假，过期时间设为-1，并调用init()方法。
init()首先判断初始化标志位，使用stat结构体获得文件描述符对应文件的信息，调用fstat方法查看当前文件描述符对应文件的状态，如果返回-1则表示有问题，初始化失败，如果返回0则表示成功，再调用S_ISSOCK函数看一下文件描述符是不是套接字，如果是把套接字标志位设为真。如果是套接字就检查一下套接字的阻塞设置。
close()
setTimeout
getTimeout
其中的属性有一些tag是否初始化，是否关闭，是否阻塞，是否是套接字，还有一个句柄（整型），一个设置的过期时间
## 其他类
不可复制接口，单例模板类，定义宏的macro，以及一些unix函数调用类如获得线程号，获得时间。
配置文件，使用yaml作为配置文件，约定大于配置，如果什么都不改就采用默认。通过偏特化使得可以通过yaml文件支持各种stl类型和基本的类型。

## http协议封装


