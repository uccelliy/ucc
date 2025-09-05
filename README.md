# UCC Server / UCC 服务器

This is a server including a **log system**, **thread pool system**, and **IO system**.  
这是一个包含 **日志系统**、**线程池系统** 和 **IO系统** 的服务器。

---

## 中文版

### 日志模块 (log4j)

日志模块在 `log.h` 中定义生成日志并输出的类，具体实现见 `log.cpp`。  
核心实现是 **格式初始化方法**，该方法接受用户自定义日志输出选项，并按照用户需求输出日志。  

日志输出器有两个子类：  
- **控制台输出器**：输出日志到控制台  
- **文件输出器**：输出日志到文件  

**难点**：  
- 使用了大量宏实现重复函数，例如各种日志格式函数和日志生成函数。

---

### 线程 / 协程模块与线程池

#### 线程模块

基于 `pthread.h`、`semaphore.h` 和 `atomic` 实现。  

- **thread.h** 中定义：  
  - 信号量 (`sem_init`, `sem_destroy`, `sem_wait`, `sem_post`)  
  - 互斥锁、自旋锁、读写锁  
  - 封装好的线程类  

- **线程创建与管理**：  
  - `pthread_create` 创建线程  
  - `pthread_join` 线程间通信  
  - `pthread_detach` 释放线程  

- 任务通过 `std::function` 传入线程构造函数，并使用 `swap` 执行。

#### 协程模块 (Fiber)

基于 `ucontext.h` 实现，在 `fiber.h` 中定义 Fiber 类。  

- **协程状态**：`INIT`、`HOLD`、`EXEC`、`TERM`、`READY`、`EXCEPT`  
- **主要方法**：  
  - `reset()` – 重新初始化协程  
  - `swapIn()` – 主线程执行协程  
  - `swapOut()` – 暂停当前协程  
  - `call()` / `back()` – 开始/返回协程  
  - 静态方法：  
    - `SetThis(Fiber* f)` / `GetThis()` – 管理主协程  
    - `YieldToReady()` / `YieldToHold()` – 改变协程状态并切换  
    - `TotalFibers()` – 返回协程总数  
    - `MainFunc()` – 执行协程回调  
    - `CallMainFunc()`  
    - `GetFiberId()`  

- 实现中使用两个静态全局线程拥有的协程作为主协程，利用 `getcontext`、`setcontext`、`makecontext`、`swapcontext`。

#### 线程池模块

在 `scheduler` 中实现，主要为 **Scheduler 类**。  

- **Scheduler 类**：  
  - 存储线程数组，协程存储在 `FiberAndThread` 列表中  
  - 根协程和静态主线程/协程  
  - 轮询调度协程  
  - 当没有任务时，执行 idle 函数  

- **主要方法**：  
  - `start()`, `stop()`  
  - `schedule(T, int)` / `scheduleNoLock(T, int)`  
  - `tickle()`, `run()`, `stopping()`, `idle()`  

- **FiberAndThread 结构体**：保存协程智能指针、函数指针和线程 ID  

---

### IO 管理模块

[细节待补充]

#### Hook 与异步 IO

[细节待补充]

---

### 网络编程

#### IP 地址

封装 **IPv4、IPv6 和 Unix 地址**。  

#### 序列化

提供序列化与反序列化，用于网络传输，底层使用 `read` 和 `write`。

#### 字节序

提供大端序和小端序的数据输入/输出函数。

#### Socket

- `socket.h`：封装 socket 函数 (`socket`, `connect`, `listen`, 等)  
- `socket_stream`：封装 IO 相关函数  

---

### 定时器

- **TimeManager** 和 **Timer** 类（互为友元）  
- Timer 抽象：`Timer` 为单个定时器，`TimeManager` 为管理类  
- 定时器行为：取消、更新、重置  
- `TimeManager` 使用最小堆维护 Timer  
- 方法 `listExpiredCb()` 检查过期定时器并执行回调  

---

### 句柄管理器

- 类：**FdCtx** 和 **FdManager**  
- **FdCtx** 管理文件描述符  
- **FdManager** 封装 FdCtx  
- 方法：  
  - `init()` – 初始化句柄信息，检查套接字、阻塞状态  
  - `close()`, `setTimeout()`, `getTimeout()`  
- 属性：初始化标志、关闭标志、阻塞标志、套接字标志、fd、超时  

---

### 其他类

- 不可复制接口  
- 单例模板类  
- 宏定义  
- Unix 工具类：获取线程号、获取时间等  
- YAML 配置文件，支持 STL 类型和基本类型  

---

### HTTP 协议封装

[细节待补充]

---

## English Version

### Log Module (log4j)

The log module defines classes in `log.h` for generating and outputting logs, implemented in `log.cpp`.  
The key implementation is the **format initialization method**, which accepts user-defined log output options and outputs logs according to user requirements.  

There are two log outputter subclasses:  
- **Console outputter**: outputs logs to the console  
- **File outputter**: outputs logs to a file  

**Challenges:**  
- Extensive use of macros to implement repetitive functions, e.g., various log format functions and log generation functions.

---

### Thread / Fiber Module and Thread Pool

#### Thread Module

Implemented using `pthread.h`, `semaphore.h`, and `atomic`.  

- **thread.h** defines:  
  - Semaphores (`sem_init`, `sem_destroy`, `sem_wait`, `sem_post`)  
  - Mutexes, spinlocks, read/write locks  
  - Thread class with encapsulated locks  

- **Thread creation and management**:  
  - `pthread_create` to create threads  
  - `pthread_join` for inter-thread communication  
  - `pthread_detach` to release threads  

- Tasks are passed via `std::function` in the thread constructor and executed using `swap`.

#### Fiber Module

Implemented using `ucontext.h` in `fiber.h`.  

- **Fiber states:** `INIT`, `HOLD`, `EXEC`, `TERM`, `READY`, `EXCEPT`  
- **Key methods:**  
  - `reset()` – re-initialize fiber  
  - `swapIn()` – start execution in main thread  
  - `swapOut()` – pause current fiber  
  - `call()` / `back()` – start or return from fiber  
  - Static methods:  
    - `SetThis(Fiber* f)` / `GetThis()` – manage main fiber  
    - `YieldToReady()` / `YieldToHold()` – change fiber state and swap  
    - `TotalFibers()` – return total fibers  
    - `MainFunc()` – execute fiber callback  
    - `CallMainFunc()`  
    - `GetFiberId()`  

- Implementation uses **two static global thread-owned fibers** as main fibers, leveraging `getcontext`, `setcontext`, `makecontext`, `swapcontext`.

#### Thread Pool Module

Implemented in the `scheduler` section.  

- **Scheduler class:**  
  - Stores threads in an array, fibers in a `FiberAndThread` list  
  - Root fiber and static main thread/fiber  
  - Round-robin scheduling for fibers  
  - Idle function executes when no tasks remain  

- **Key methods:**  
  - `start()`, `stop()`  
  - `schedule(T, int)` / `scheduleNoLock(T, int)`  
  - `tickle()`, `run()`, `stopping()`, `idle()`  

- **FiberAndThread struct:** stores fiber smart pointer, function pointer, and thread ID  

---

### IO Manager

[Details pending.]

#### Hook and Asynchronous IO

[Details pending.]

---

### Network Programming

#### IP Address

Encapsulates **IPv4, IPv6, and Unix addresses**.  

#### Serialization

Provides serialization and deserialization for network transmission, using low-level `read` and `write`.

#### Byte Order

Functions for **big-endian** and **little-endian** data input/output.

#### Socket

- `socket.h`: encapsulates socket functions (`socket`, `connect`, `listen`, etc.)  
- `socket_stream`: encapsulates IO-related functions  

---

### Timer

- **TimeManager** and **Timer** classes (friend classes)  
- Timer abstraction: `Timer` for single timers, `TimerManager` as manager  
- Timer behaviors: cancel, update, reset  
- `TimeManager` maintains a **min-heap** of timers  
- Method `listExpiredCb()` checks expired timers and executes callbacks  

---

### Handle Manager

- Classes: **FdCtx** and **FdManager**  
- **FdCtx** manages a file descriptor  
- **FdManager** wraps FdCtx  
- Methods:  
  - `init()` – initialize handle info, check socket, blocking mode  
  - `close()`, `setTimeout()`, `getTimeout()`  
- Attributes: initialized flag, closed flag, blocking flag, socket flag, fd, timeout  

---

### Other Classes

- Non-copyable interfaces  
- Singleton template class  
- Macro definitions  
- Unix utility classes: get thread ID, get time, etc.  
- YAML configuration file, supports STL types and basic types  

---

### HTTP Protocol Encapsulation

[Details pending.]

