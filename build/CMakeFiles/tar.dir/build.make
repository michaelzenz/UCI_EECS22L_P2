# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/michaelz/Desktop/projects/courses/22L/k-chat-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michaelz/Desktop/projects/courses/22L/k-chat-server/build

# Utility rule file for tar.

# Include the progress variables for this target.
include CMakeFiles/tar.dir/progress.make

CMakeFiles/tar:
	/usr/local/bin/cmake -E tar zcvf ../source.tar.gz /home/michaelz/Desktop/projects/courses/22L/k-chat-server/include /home/michaelz/Desktop/projects/courses/22L/k-chat-server/lib /home/michaelz/Desktop/projects/courses/22L/k-chat-server/res /home/michaelz/Desktop/projects/courses/22L/k-chat-server/src /home/michaelz/Desktop/projects/courses/22L/k-chat-server/CMakeLists.txt /home/michaelz/Desktop/projects/courses/22L/k-chat-server/jsmn\ LICENSE /home/michaelz/Desktop/projects/courses/22L/k-chat-server/SuperSimpleInstallAndRun.sh

tar: CMakeFiles/tar
tar: CMakeFiles/tar.dir/build.make

.PHONY : tar

# Rule to build all files generated by this target.
CMakeFiles/tar.dir/build: tar

.PHONY : CMakeFiles/tar.dir/build

CMakeFiles/tar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tar.dir/clean

CMakeFiles/tar.dir/depend:
	cd /home/michaelz/Desktop/projects/courses/22L/k-chat-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michaelz/Desktop/projects/courses/22L/k-chat-server /home/michaelz/Desktop/projects/courses/22L/k-chat-server /home/michaelz/Desktop/projects/courses/22L/k-chat-server/build /home/michaelz/Desktop/projects/courses/22L/k-chat-server/build /home/michaelz/Desktop/projects/courses/22L/k-chat-server/build/CMakeFiles/tar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tar.dir/depend

