# ucc
study project
2023年12月5日，写完这个项目也一个月了近，写的时候囫囵吞枣，必须细细再品一遍。今天是实习第一天，发现自己远远不够，总是能学到很多东西，希望自己不要疲惫，不要畏惧。多多学习，谦虚学习。这里会包含这个服务器的类图，也会包含里面的一些计算机基础知识，包含里面一些编程思想，总之所有在回顾期间我不会的东西，都会写在这个上面。加油。

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


##
