# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/armando/Desktop/project/kchat-client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/armando/Desktop/project/kchat-client/build

# Include any dependencies generated for this target.
include CMakeFiles/kchat-client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kchat-client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kchat-client.dir/flags.make

CMakeFiles/kchat-client.dir/src/jsmn.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/jsmn.c.o: ../src/jsmn.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kchat-client.dir/src/jsmn.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/jsmn.c.o   -c /home/armando/Desktop/project/kchat-client/src/jsmn.c

CMakeFiles/kchat-client.dir/src/jsmn.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/jsmn.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/jsmn.c > CMakeFiles/kchat-client.dir/src/jsmn.c.i

CMakeFiles/kchat-client.dir/src/jsmn.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/jsmn.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/jsmn.c -o CMakeFiles/kchat-client.dir/src/jsmn.c.s

CMakeFiles/kchat-client.dir/src/jsmn.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/jsmn.c.o.requires

CMakeFiles/kchat-client.dir/src/jsmn.c.o.provides: CMakeFiles/kchat-client.dir/src/jsmn.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/jsmn.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/jsmn.c.o.provides

CMakeFiles/kchat-client.dir/src/jsmn.c.o.provides.build: CMakeFiles/kchat-client.dir/src/jsmn.c.o


CMakeFiles/kchat-client.dir/src/stack.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/stack.c.o: ../src/stack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/kchat-client.dir/src/stack.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/stack.c.o   -c /home/armando/Desktop/project/kchat-client/src/stack.c

CMakeFiles/kchat-client.dir/src/stack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/stack.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/stack.c > CMakeFiles/kchat-client.dir/src/stack.c.i

CMakeFiles/kchat-client.dir/src/stack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/stack.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/stack.c -o CMakeFiles/kchat-client.dir/src/stack.c.s

CMakeFiles/kchat-client.dir/src/stack.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/stack.c.o.requires

CMakeFiles/kchat-client.dir/src/stack.c.o.provides: CMakeFiles/kchat-client.dir/src/stack.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/stack.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/stack.c.o.provides

CMakeFiles/kchat-client.dir/src/stack.c.o.provides.build: CMakeFiles/kchat-client.dir/src/stack.c.o


CMakeFiles/kchat-client.dir/src/encode.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/encode.c.o: ../src/encode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/kchat-client.dir/src/encode.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/encode.c.o   -c /home/armando/Desktop/project/kchat-client/src/encode.c

CMakeFiles/kchat-client.dir/src/encode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/encode.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/encode.c > CMakeFiles/kchat-client.dir/src/encode.c.i

CMakeFiles/kchat-client.dir/src/encode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/encode.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/encode.c -o CMakeFiles/kchat-client.dir/src/encode.c.s

CMakeFiles/kchat-client.dir/src/encode.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/encode.c.o.requires

CMakeFiles/kchat-client.dir/src/encode.c.o.provides: CMakeFiles/kchat-client.dir/src/encode.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/encode.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/encode.c.o.provides

CMakeFiles/kchat-client.dir/src/encode.c.o.provides.build: CMakeFiles/kchat-client.dir/src/encode.c.o


CMakeFiles/kchat-client.dir/src/ENV.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/ENV.c.o: ../src/ENV.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/kchat-client.dir/src/ENV.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/ENV.c.o   -c /home/armando/Desktop/project/kchat-client/src/ENV.c

CMakeFiles/kchat-client.dir/src/ENV.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/ENV.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/ENV.c > CMakeFiles/kchat-client.dir/src/ENV.c.i

CMakeFiles/kchat-client.dir/src/ENV.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/ENV.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/ENV.c -o CMakeFiles/kchat-client.dir/src/ENV.c.s

CMakeFiles/kchat-client.dir/src/ENV.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/ENV.c.o.requires

CMakeFiles/kchat-client.dir/src/ENV.c.o.provides: CMakeFiles/kchat-client.dir/src/ENV.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/ENV.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/ENV.c.o.provides

CMakeFiles/kchat-client.dir/src/ENV.c.o.provides.build: CMakeFiles/kchat-client.dir/src/ENV.c.o


CMakeFiles/kchat-client.dir/src/GUIO.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/GUIO.c.o: ../src/GUIO.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/kchat-client.dir/src/GUIO.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/GUIO.c.o   -c /home/armando/Desktop/project/kchat-client/src/GUIO.c

CMakeFiles/kchat-client.dir/src/GUIO.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/GUIO.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/GUIO.c > CMakeFiles/kchat-client.dir/src/GUIO.c.i

CMakeFiles/kchat-client.dir/src/GUIO.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/GUIO.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/GUIO.c -o CMakeFiles/kchat-client.dir/src/GUIO.c.s

CMakeFiles/kchat-client.dir/src/GUIO.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/GUIO.c.o.requires

CMakeFiles/kchat-client.dir/src/GUIO.c.o.provides: CMakeFiles/kchat-client.dir/src/GUIO.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/GUIO.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/GUIO.c.o.provides

CMakeFiles/kchat-client.dir/src/GUIO.c.o.provides.build: CMakeFiles/kchat-client.dir/src/GUIO.c.o


CMakeFiles/kchat-client.dir/src/decode.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/decode.c.o: ../src/decode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/kchat-client.dir/src/decode.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/decode.c.o   -c /home/armando/Desktop/project/kchat-client/src/decode.c

CMakeFiles/kchat-client.dir/src/decode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/decode.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/decode.c > CMakeFiles/kchat-client.dir/src/decode.c.i

CMakeFiles/kchat-client.dir/src/decode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/decode.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/decode.c -o CMakeFiles/kchat-client.dir/src/decode.c.s

CMakeFiles/kchat-client.dir/src/decode.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/decode.c.o.requires

CMakeFiles/kchat-client.dir/src/decode.c.o.provides: CMakeFiles/kchat-client.dir/src/decode.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/decode.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/decode.c.o.provides

CMakeFiles/kchat-client.dir/src/decode.c.o.provides.build: CMakeFiles/kchat-client.dir/src/decode.c.o


CMakeFiles/kchat-client.dir/src/GUI.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/GUI.c.o: ../src/GUI.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/kchat-client.dir/src/GUI.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/GUI.c.o   -c /home/armando/Desktop/project/kchat-client/src/GUI.c

CMakeFiles/kchat-client.dir/src/GUI.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/GUI.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/GUI.c > CMakeFiles/kchat-client.dir/src/GUI.c.i

CMakeFiles/kchat-client.dir/src/GUI.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/GUI.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/GUI.c -o CMakeFiles/kchat-client.dir/src/GUI.c.s

CMakeFiles/kchat-client.dir/src/GUI.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/GUI.c.o.requires

CMakeFiles/kchat-client.dir/src/GUI.c.o.provides: CMakeFiles/kchat-client.dir/src/GUI.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/GUI.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/GUI.c.o.provides

CMakeFiles/kchat-client.dir/src/GUI.c.o.provides.build: CMakeFiles/kchat-client.dir/src/GUI.c.o


CMakeFiles/kchat-client.dir/src/vector.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/vector.c.o: ../src/vector.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/kchat-client.dir/src/vector.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/vector.c.o   -c /home/armando/Desktop/project/kchat-client/src/vector.c

CMakeFiles/kchat-client.dir/src/vector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/vector.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/vector.c > CMakeFiles/kchat-client.dir/src/vector.c.i

CMakeFiles/kchat-client.dir/src/vector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/vector.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/vector.c -o CMakeFiles/kchat-client.dir/src/vector.c.s

CMakeFiles/kchat-client.dir/src/vector.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/vector.c.o.requires

CMakeFiles/kchat-client.dir/src/vector.c.o.provides: CMakeFiles/kchat-client.dir/src/vector.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/vector.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/vector.c.o.provides

CMakeFiles/kchat-client.dir/src/vector.c.o.provides.build: CMakeFiles/kchat-client.dir/src/vector.c.o


CMakeFiles/kchat-client.dir/src/main.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/kchat-client.dir/src/main.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/main.c.o   -c /home/armando/Desktop/project/kchat-client/src/main.c

CMakeFiles/kchat-client.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/main.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/main.c > CMakeFiles/kchat-client.dir/src/main.c.i

CMakeFiles/kchat-client.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/main.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/main.c -o CMakeFiles/kchat-client.dir/src/main.c.s

CMakeFiles/kchat-client.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/main.c.o.requires

CMakeFiles/kchat-client.dir/src/main.c.o.provides: CMakeFiles/kchat-client.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/main.c.o.provides

CMakeFiles/kchat-client.dir/src/main.c.o.provides.build: CMakeFiles/kchat-client.dir/src/main.c.o


CMakeFiles/kchat-client.dir/src/connection.c.o: CMakeFiles/kchat-client.dir/flags.make
CMakeFiles/kchat-client.dir/src/connection.c.o: ../src/connection.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/kchat-client.dir/src/connection.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kchat-client.dir/src/connection.c.o   -c /home/armando/Desktop/project/kchat-client/src/connection.c

CMakeFiles/kchat-client.dir/src/connection.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kchat-client.dir/src/connection.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/armando/Desktop/project/kchat-client/src/connection.c > CMakeFiles/kchat-client.dir/src/connection.c.i

CMakeFiles/kchat-client.dir/src/connection.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kchat-client.dir/src/connection.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/armando/Desktop/project/kchat-client/src/connection.c -o CMakeFiles/kchat-client.dir/src/connection.c.s

CMakeFiles/kchat-client.dir/src/connection.c.o.requires:

.PHONY : CMakeFiles/kchat-client.dir/src/connection.c.o.requires

CMakeFiles/kchat-client.dir/src/connection.c.o.provides: CMakeFiles/kchat-client.dir/src/connection.c.o.requires
	$(MAKE) -f CMakeFiles/kchat-client.dir/build.make CMakeFiles/kchat-client.dir/src/connection.c.o.provides.build
.PHONY : CMakeFiles/kchat-client.dir/src/connection.c.o.provides

CMakeFiles/kchat-client.dir/src/connection.c.o.provides.build: CMakeFiles/kchat-client.dir/src/connection.c.o


# Object files for target kchat-client
kchat__client_OBJECTS = \
"CMakeFiles/kchat-client.dir/src/jsmn.c.o" \
"CMakeFiles/kchat-client.dir/src/stack.c.o" \
"CMakeFiles/kchat-client.dir/src/encode.c.o" \
"CMakeFiles/kchat-client.dir/src/ENV.c.o" \
"CMakeFiles/kchat-client.dir/src/GUIO.c.o" \
"CMakeFiles/kchat-client.dir/src/decode.c.o" \
"CMakeFiles/kchat-client.dir/src/GUI.c.o" \
"CMakeFiles/kchat-client.dir/src/vector.c.o" \
"CMakeFiles/kchat-client.dir/src/main.c.o" \
"CMakeFiles/kchat-client.dir/src/connection.c.o"

# External object files for target kchat-client
kchat__client_EXTERNAL_OBJECTS =

../bin/kchat-client: CMakeFiles/kchat-client.dir/src/jsmn.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/stack.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/encode.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/ENV.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/GUIO.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/decode.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/GUI.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/vector.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/main.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/src/connection.c.o
../bin/kchat-client: CMakeFiles/kchat-client.dir/build.make
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libglib-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgobject-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libatk-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgio-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgthread-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgmodule-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgdk_pixbuf-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libcairo.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpango-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangocairo-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangoft2-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangoxft-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgdk-x11-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgtk-x11-2.0.so
../bin/kchat-client: src/AI/libAI_LIB.a
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libglib-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgobject-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libatk-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgio-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgthread-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgmodule-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgdk_pixbuf-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libcairo.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpango-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangocairo-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangoft2-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libpangoxft-1.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgdk-x11-2.0.so
../bin/kchat-client: /usr/lib/x86_64-linux-gnu/libgtk-x11-2.0.so
../bin/kchat-client: CMakeFiles/kchat-client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/armando/Desktop/project/kchat-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable ../bin/kchat-client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kchat-client.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy_directory /home/armando/Desktop/project/kchat-client/res /home/armando/Desktop/project/kchat-client/bin/res

# Rule to build all files generated by this target.
CMakeFiles/kchat-client.dir/build: ../bin/kchat-client

.PHONY : CMakeFiles/kchat-client.dir/build

CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/jsmn.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/stack.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/encode.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/ENV.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/GUIO.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/decode.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/GUI.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/vector.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/main.c.o.requires
CMakeFiles/kchat-client.dir/requires: CMakeFiles/kchat-client.dir/src/connection.c.o.requires

.PHONY : CMakeFiles/kchat-client.dir/requires

CMakeFiles/kchat-client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kchat-client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kchat-client.dir/clean

CMakeFiles/kchat-client.dir/depend:
	cd /home/armando/Desktop/project/kchat-client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/armando/Desktop/project/kchat-client /home/armando/Desktop/project/kchat-client /home/armando/Desktop/project/kchat-client/build /home/armando/Desktop/project/kchat-client/build /home/armando/Desktop/project/kchat-client/build/CMakeFiles/kchat-client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kchat-client.dir/depend

