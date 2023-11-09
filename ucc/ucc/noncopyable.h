#ifndef __UCC_NONCOPYABLE_H__
#define __UCC_NONCOPYABLE_H__

namespace ucc{

class Noncopyable{
public:
	Noncopyable() = default;
	~Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;

};
}

#endif
