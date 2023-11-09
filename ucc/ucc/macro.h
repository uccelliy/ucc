#ifndef __UCC_MACRO_H__
#define __UCC_MACRO_H__

#include<string.h>
#include<assert.h>
#include"util.h"

#if defined __GNUC__ || defined __llvm__
#	define UCC_LICKLY(x)   __builtin_expect(!!(x),1)
#	define UCC_UNLICKLY(x) __builtin_expect(!!(x),0)
#else
#	define UCC_LICKLY(x)   (x)
#	define UCC_UNLICKLY(x) (x)
#endif

#define UCC_ASSERT(x) \
	if(UCC_UNLICKLY(!(x))){ \
		UCC_LOG_ERROR(UCC_LOG_ROOT())<<"ASSERTION: " #x \
			<<"\nbacktrace:\n" \
		    << ucc::BacktraceToString(100,2,"    "); \
		assert(x); \
	}

#define UCC_ASSERT2(x,w) \
	if(UCC_UNLICKLY(!(x))){ \
		UCC_LOG_ERROR(UCC_LOG_ROOT())<<"ASSERTION: " #x \
		    <<"\n"<<w \
			<<"\nbacktrace:\n" \
		    << ucc::BacktraceToString(100,2,"    "); \
		assert(x); \
	}

#endif
