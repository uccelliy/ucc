cmake_minimum_required(VERSION 2.8)
project(ucc)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    ucc/log.cpp
	ucc/util.cpp
	ucc/config.cpp
   )

add_library(ucc SHARED ${LIB_SRC})
#add_library(ucc_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (ucc_static PROPERTIES OUTPUT_NAME "ucc")

add_executable(test tests/test.cpp)
add_dependencies(test ucc)
target_link_libraries(test ucc)

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config ucc)
target_link_libraries(test_config ucc)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
