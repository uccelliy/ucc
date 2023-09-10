#ifndef __UCC_UTIL_H__
#define __UCC_UTIL_H__

#include<stdint.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<unistd.h>
namespace ucc{

pid_t GetThreadId();
uint32_t GetFiberId();
}




#endif
