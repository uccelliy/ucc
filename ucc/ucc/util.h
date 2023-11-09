#ifndef __UCC_UTIL_H__
#define __UCC_UTIL_H__

#include<vector>
#include<string>
#include<stdint.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<unistd.h>
namespace ucc{

pid_t GetThreadId();
uint32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt , int size =64 , int skip=1);

std::string BacktraceToString(int size = 64,  int skip=2, const std::string& prefix = " ");

uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

}




#endif
