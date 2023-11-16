cmake_minimum_required(VERSION 2.8)
project(ucc)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)
include_directories(/home/yinruochen/yaml-cpp/yaml-cpp/include)

link_directories(/home/yinruochen/yaml-cpp/yaml-cpp/build)

set(LIB_SRC
    ucc/log.cpp
	ucc/util.cpp
	ucc/config.cpp
	ucc/thread.cpp
	ucc/fiber.cpp
	ucc/scheduler.cpp
	ucc/iomanager.cpp
	ucc/timer.cpp
	ucc/hook.cpp
	ucc/fd_manager.cpp
	ucc/bytearray.cpp
	ucc/address.cpp
	ucc/socket.cpp
	ucc/tcp_server.cpp
	ucc/http/http.cpp
	ucc/http/http11_parser.rl.cpp
	ucc/http/httpclient_parser.rl.cpp
	ucc/http/http_parser.cpp
	ucc/http/http_session.cpp
	ucc/http/http_server.cpp
	ucc/http/http_connection.cpp
	ucc/http/servlet.cpp
	ucc/stream.cpp
	ucc/socket_stream.cpp
   )

add_library(ucc SHARED ${LIB_SRC})
#add_library(ucc_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (ucc_static PROPERTIES OUTPUT_NAME "ucc")

set(LIB_LIB
	ucc
	pthread
	yaml-cpp
	dl
	)

add_executable(test tests/test.cpp)
add_dependencies(test ucc)
target_link_libraries(test ucc ${LIB_LIB}) 

add_executable(echo_server examples/echo_server.cpp)
add_dependencies(echo_server ucc)
target_link_libraries(echo_server ucc ${LIB_LIB}) 

add_executable(test_tcp_server tests/test_tcp_server.cpp)
add_dependencies(test_tcp_server ucc)
target_link_libraries(test_tcp_server ucc ${LIB_LIB}) 

add_executable(test_http_connection tests/test_http_connection.cpp)
add_dependencies(test_http_connection ucc)
target_link_libraries(test_http_connection ucc ${LIB_LIB}) 


add_executable(test_http_server tests/test_http_server.cpp)
add_dependencies(test_http_server ucc)
target_link_libraries(test_http_server ucc ${LIB_LIB}) 

add_executable(test_http tests/test_http.cpp)
add_dependencies(test_http ucc)
target_link_libraries(test_http ucc ${LIB_LIB}) 

add_executable(test_http_parser tests/test_http_parser.cpp)
add_dependencies(test_http_parser ucc)
target_link_libraries(test_http_parser ucc ${LIB_LIB}) 

add_executable(test_bytearray tests/test_bytearray.cpp)
add_dependencies(test_bytearray ucc)
target_link_libraries(test_bytearray ucc ${LIB_LIB}) 

add_executable(test_socket tests/test_socket.cpp)
add_dependencies(test_socket ucc)
target_link_libraries(test_socket ucc ${LIB_LIB}) 

add_executable(test_address tests/test_address.cpp)
add_dependencies(test_address ucc)
target_link_libraries(test_address ucc ${LIB_LIB}) 


add_executable(test_util tests/test_util.cpp)
add_dependencies(test_util ucc)
target_link_libraries(test_util ucc ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cpp)
add_dependencies(test_fiber ucc)
target_link_libraries(test_fiber ucc ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cpp)
add_dependencies(test_scheduler ucc)
target_link_libraries(test_scheduler ucc ${LIB_LIB})

add_executable(test_iomanager tests/test_iomanager.cpp)
add_dependencies(test_iomanager ucc)
target_link_libraries(test_iomanager ucc ${LIB_LIB}) 

add_executable(test_hook tests/test_hook.cpp)
add_dependencies(test_hook ucc)
target_link_libraries(test_hook ucc ${LIB_LIB}) 



add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config ucc)
target_link_libraries(test_config ucc ${LIB_LIB})

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread ucc)
target_link_libraries(test_thread ucc ${LIB_LIB})



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
