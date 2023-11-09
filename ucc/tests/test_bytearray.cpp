#include"../ucc/bytearray.h"
#include"../ucc/ucc.h"

static ucc::Logger::ptr g_logger = UCC_LOG_ROOT();

void test(){
#define XX(type,len,write_fun,read_fun,base_len) {\
	std::vector<type> vec; \
	for(int i = 0; i<len ; i++){ \
		vec.push_back(rand()); \
	} \
	ucc::ByteArray::ptr ba(new ucc::ByteArray(base_len)); \
	for(auto&i : vec){ \
		ba->write_fun(i); \
	} \
	ba->setPosition(0); \
	for(size_t i = 0 ; i<vec.size() ; ++i){ \
		type v = ba->read_fun(); \
		UCC_ASSERT(v == vec[i]); \
	} \
	UCC_ASSERT(ba->getReadSize() == 0); \
	UCC_LOG_INFO(g_logger)<<#write_fun "/" #read_fun "(" #type ")len=" <<len \
				<<"base_len="<<base_len<<"size="<<ba->getSize(); \
}

	XX(int8_t , 100 , writeFint8 , readFint8 , 1);
	XX(uint16_t , 100 , writeFuint16 , readFuint16 ,1);
	XX(uint64_t , 100 , writeFuint64 , readFuint64 ,1);
	XX(int64_t , 100 , writeFint64 , readFint64 ,1);
	XX(int64_t , 100 , writeInt64 , readInt64 , 1);
	XX(uint64_t , 100 , writeUint64 , readUint64 ,1);
#undef XX

#define XX(type,len,write_fun,read_fun,base_len) {\
	std::vector<type> vec; \
	for(int i = 0; i<len ; i++){ \
		vec.push_back(rand()); \
	} \
	ucc::ByteArray::ptr ba(new ucc::ByteArray(base_len)); \
	for(auto&i : vec){ \
		ba->write_fun(i); \
	} \
	ba->setPosition(0); \
	for(size_t i = 0 ; i<vec.size() ; ++i){ \
		type v = ba->read_fun(); \
		UCC_ASSERT(v == vec[i]); \
	} \
	UCC_ASSERT(ba->getReadSize() == 0); \
	UCC_LOG_INFO(g_logger)<<#write_fun "/" #read_fun "(" #type ")len=" <<len \
				<<"base_len="<<base_len<<"size="<<ba->getSize(); \
	ba->setPosition(0); \
	UCC_ASSERT(ba->writeToFile( #type "_" #len ".dat")); \
	ucc::ByteArray::ptr ba2(new ucc::ByteArray(base_len * 2)); \
	UCC_ASSERT(ba2->readFromFile( #type "_" #len ".dat")); \
	ba2->setPosition(0); \
	UCC_ASSERT(ba->toString() == ba2->toString()); \
	UCC_ASSERT(ba->getPosition() == 0); \
	UCC_ASSERT(ba2->getPosition() == 0); \
}
	XX(int8_t , 100 , writeFint8 , readFint8 , 1);
	XX(uint16_t , 100 , writeFuint16 , readFuint16 ,1);
	XX(uint64_t , 100 , writeFuint64 , readFuint64 ,1);
	XX(int64_t , 100 , writeFint64 , readFint64 ,1);
	XX(int64_t , 100 , writeInt64 , readInt64 , 1);
	XX(uint64_t , 100 , writeUint64 , readUint64 ,1);

#undef XX
}

int main(int argc, char** argv){
	test();
	return 0;
}
