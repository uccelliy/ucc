# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/yinruochen/Downloads/cmake-3.27.4-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/yinruochen/Downloads/cmake-3.27.4-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yinruochen/workspace/ucc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yinruochen/workspace/ucc/build

# Include any dependencies generated for this target.
include CMakeFiles/test_http_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_http_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_http_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_http_server.dir/flags.make

CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o: CMakeFiles/test_http_server.dir/flags.make
CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o: /home/yinruochen/workspace/ucc/tests/test_http_server.cpp
CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o: CMakeFiles/test_http_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yinruochen/workspace/ucc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o -MF CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o.d -o CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o -c /home/yinruochen/workspace/ucc/tests/test_http_server.cpp

CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yinruochen/workspace/ucc/tests/test_http_server.cpp > CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.i

CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yinruochen/workspace/ucc/tests/test_http_server.cpp -o CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.s

# Object files for target test_http_server
test_http_server_OBJECTS = \
"CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o"

# External object files for target test_http_server
test_http_server_EXTERNAL_OBJECTS =

/home/yinruochen/workspace/ucc/bin/test_http_server: CMakeFiles/test_http_server.dir/tests/test_http_server.cpp.o
/home/yinruochen/workspace/ucc/bin/test_http_server: CMakeFiles/test_http_server.dir/build.make
/home/yinruochen/workspace/ucc/bin/test_http_server: /home/yinruochen/workspace/ucc/lib/libucc.so
/home/yinruochen/workspace/ucc/bin/test_http_server: CMakeFiles/test_http_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yinruochen/workspace/ucc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/yinruochen/workspace/ucc/bin/test_http_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_http_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_http_server.dir/build: /home/yinruochen/workspace/ucc/bin/test_http_server
.PHONY : CMakeFiles/test_http_server.dir/build

CMakeFiles/test_http_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_http_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_http_server.dir/clean

CMakeFiles/test_http_server.dir/depend:
	cd /home/yinruochen/workspace/ucc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yinruochen/workspace/ucc /home/yinruochen/workspace/ucc /home/yinruochen/workspace/ucc/build /home/yinruochen/workspace/ucc/build /home/yinruochen/workspace/ucc/build/CMakeFiles/test_http_server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_http_server.dir/depend

