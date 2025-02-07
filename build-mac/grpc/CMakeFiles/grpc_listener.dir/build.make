# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/marcusforte/workspace/repos/learning-opengl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/marcusforte/workspace/repos/learning-opengl/build-mac

# Include any dependencies generated for this target.
include grpc/CMakeFiles/grpc_listener.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include grpc/CMakeFiles/grpc_listener.dir/compiler_depend.make

# Include the progress variables for this target.
include grpc/CMakeFiles/grpc_listener.dir/progress.make

# Include the compile flags for this target's objects.
include grpc/CMakeFiles/grpc_listener.dir/flags.make

grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o: grpc/CMakeFiles/grpc_listener.dir/flags.make
grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o: /Users/marcusforte/workspace/repos/learning-opengl/grpc/grpc_listener.cc
grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o: grpc/CMakeFiles/grpc_listener.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/marcusforte/workspace/repos/learning-opengl/build-mac/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o"
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o -MF CMakeFiles/grpc_listener.dir/grpc_listener.cc.o.d -o CMakeFiles/grpc_listener.dir/grpc_listener.cc.o -c /Users/marcusforte/workspace/repos/learning-opengl/grpc/grpc_listener.cc

grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/grpc_listener.dir/grpc_listener.cc.i"
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/marcusforte/workspace/repos/learning-opengl/grpc/grpc_listener.cc > CMakeFiles/grpc_listener.dir/grpc_listener.cc.i

grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/grpc_listener.dir/grpc_listener.cc.s"
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/marcusforte/workspace/repos/learning-opengl/grpc/grpc_listener.cc -o CMakeFiles/grpc_listener.dir/grpc_listener.cc.s

# Object files for target grpc_listener
grpc_listener_OBJECTS = \
"CMakeFiles/grpc_listener.dir/grpc_listener.cc.o"

# External object files for target grpc_listener
grpc_listener_EXTERNAL_OBJECTS =

grpc/libgrpc_listener.a: grpc/CMakeFiles/grpc_listener.dir/grpc_listener.cc.o
grpc/libgrpc_listener.a: grpc/CMakeFiles/grpc_listener.dir/build.make
grpc/libgrpc_listener.a: grpc/CMakeFiles/grpc_listener.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/marcusforte/workspace/repos/learning-opengl/build-mac/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgrpc_listener.a"
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc_listener.dir/cmake_clean_target.cmake
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grpc_listener.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
grpc/CMakeFiles/grpc_listener.dir/build: grpc/libgrpc_listener.a
.PHONY : grpc/CMakeFiles/grpc_listener.dir/build

grpc/CMakeFiles/grpc_listener.dir/clean:
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc && $(CMAKE_COMMAND) -P CMakeFiles/grpc_listener.dir/cmake_clean.cmake
.PHONY : grpc/CMakeFiles/grpc_listener.dir/clean

grpc/CMakeFiles/grpc_listener.dir/depend:
	cd /Users/marcusforte/workspace/repos/learning-opengl/build-mac && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/marcusforte/workspace/repos/learning-opengl /Users/marcusforte/workspace/repos/learning-opengl/grpc /Users/marcusforte/workspace/repos/learning-opengl/build-mac /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc /Users/marcusforte/workspace/repos/learning-opengl/build-mac/grpc/CMakeFiles/grpc_listener.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : grpc/CMakeFiles/grpc_listener.dir/depend

