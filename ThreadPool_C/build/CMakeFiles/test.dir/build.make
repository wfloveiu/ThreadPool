# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wufang/Tinywebserver/ThreadPool_learning

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wufang/Tinywebserver/ThreadPool_learning/build

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/src/main.c.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/src/main.c.o: ../src/main.c
CMakeFiles/test.dir/src/main.c.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wufang/Tinywebserver/ThreadPool_learning/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test.dir/src/main.c.o -MF CMakeFiles/test.dir/src/main.c.o.d -o CMakeFiles/test.dir/src/main.c.o -c /home/wufang/Tinywebserver/ThreadPool_learning/src/main.c

CMakeFiles/test.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wufang/Tinywebserver/ThreadPool_learning/src/main.c > CMakeFiles/test.dir/src/main.c.i

CMakeFiles/test.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wufang/Tinywebserver/ThreadPool_learning/src/main.c -o CMakeFiles/test.dir/src/main.c.s

CMakeFiles/test.dir/src/threadpool.c.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/src/threadpool.c.o: ../src/threadpool.c
CMakeFiles/test.dir/src/threadpool.c.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wufang/Tinywebserver/ThreadPool_learning/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test.dir/src/threadpool.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test.dir/src/threadpool.c.o -MF CMakeFiles/test.dir/src/threadpool.c.o.d -o CMakeFiles/test.dir/src/threadpool.c.o -c /home/wufang/Tinywebserver/ThreadPool_learning/src/threadpool.c

CMakeFiles/test.dir/src/threadpool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/src/threadpool.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wufang/Tinywebserver/ThreadPool_learning/src/threadpool.c > CMakeFiles/test.dir/src/threadpool.c.i

CMakeFiles/test.dir/src/threadpool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/src/threadpool.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wufang/Tinywebserver/ThreadPool_learning/src/threadpool.c -o CMakeFiles/test.dir/src/threadpool.c.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/src/main.c.o" \
"CMakeFiles/test.dir/src/threadpool.c.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

../bin/test: CMakeFiles/test.dir/src/main.c.o
../bin/test: CMakeFiles/test.dir/src/threadpool.c.o
../bin/test: CMakeFiles/test.dir/build.make
../bin/test: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wufang/Tinywebserver/ThreadPool_learning/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../bin/test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: ../bin/test
.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	cd /home/wufang/Tinywebserver/ThreadPool_learning/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wufang/Tinywebserver/ThreadPool_learning /home/wufang/Tinywebserver/ThreadPool_learning /home/wufang/Tinywebserver/ThreadPool_learning/build /home/wufang/Tinywebserver/ThreadPool_learning/build /home/wufang/Tinywebserver/ThreadPool_learning/build/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test.dir/depend
