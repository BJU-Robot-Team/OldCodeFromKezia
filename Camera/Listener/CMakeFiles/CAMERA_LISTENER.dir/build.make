# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/Kezia/Camera/Listener

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Kezia/Camera/Listener

# Include any dependencies generated for this target.
include CMakeFiles/CAMERA_LISTENER.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CAMERA_LISTENER.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CAMERA_LISTENER.dir/flags.make

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o: CMakeFiles/CAMERA_LISTENER.dir/flags.make
CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o: Camera_Listener.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/Kezia/Camera/Listener/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o -c /home/user/Kezia/Camera/Listener/Camera_Listener.cpp

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/user/Kezia/Camera/Listener/Camera_Listener.cpp > CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.i

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/user/Kezia/Camera/Listener/Camera_Listener.cpp -o CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.s

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.requires:
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.requires

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.provides: CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.requires
	$(MAKE) -f CMakeFiles/CAMERA_LISTENER.dir/build.make CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.provides.build
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.provides

CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.provides.build: CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o

# Object files for target CAMERA_LISTENER
CAMERA_LISTENER_OBJECTS = \
"CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o"

# External object files for target CAMERA_LISTENER
CAMERA_LISTENER_EXTERNAL_OBJECTS =

CAMERA_LISTENER: CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o
CAMERA_LISTENER: CMakeFiles/CAMERA_LISTENER.dir/build.make
CAMERA_LISTENER: CMakeFiles/CAMERA_LISTENER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable CAMERA_LISTENER"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CAMERA_LISTENER.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CAMERA_LISTENER.dir/build: CAMERA_LISTENER
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/build

CMakeFiles/CAMERA_LISTENER.dir/requires: CMakeFiles/CAMERA_LISTENER.dir/Camera_Listener.cpp.o.requires
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/requires

CMakeFiles/CAMERA_LISTENER.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CAMERA_LISTENER.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/clean

CMakeFiles/CAMERA_LISTENER.dir/depend:
	cd /home/user/Kezia/Camera/Listener && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Kezia/Camera/Listener /home/user/Kezia/Camera/Listener /home/user/Kezia/Camera/Listener /home/user/Kezia/Camera/Listener /home/user/Kezia/Camera/Listener/CMakeFiles/CAMERA_LISTENER.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CAMERA_LISTENER.dir/depend

