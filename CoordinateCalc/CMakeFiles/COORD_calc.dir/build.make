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
CMAKE_SOURCE_DIR = /home/user/Kezia/CoordinateCalc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Kezia/CoordinateCalc

# Include any dependencies generated for this target.
include CMakeFiles/COORD_calc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/COORD_calc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/COORD_calc.dir/flags.make

CMakeFiles/COORD_calc.dir/coordcalc.cpp.o: CMakeFiles/COORD_calc.dir/flags.make
CMakeFiles/COORD_calc.dir/coordcalc.cpp.o: coordcalc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/Kezia/CoordinateCalc/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/COORD_calc.dir/coordcalc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/COORD_calc.dir/coordcalc.cpp.o -c /home/user/Kezia/CoordinateCalc/coordcalc.cpp

CMakeFiles/COORD_calc.dir/coordcalc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/COORD_calc.dir/coordcalc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/user/Kezia/CoordinateCalc/coordcalc.cpp > CMakeFiles/COORD_calc.dir/coordcalc.cpp.i

CMakeFiles/COORD_calc.dir/coordcalc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/COORD_calc.dir/coordcalc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/user/Kezia/CoordinateCalc/coordcalc.cpp -o CMakeFiles/COORD_calc.dir/coordcalc.cpp.s

CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.requires:
.PHONY : CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.requires

CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.provides: CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.requires
	$(MAKE) -f CMakeFiles/COORD_calc.dir/build.make CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.provides.build
.PHONY : CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.provides

CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.provides.build: CMakeFiles/COORD_calc.dir/coordcalc.cpp.o

# Object files for target COORD_calc
COORD_calc_OBJECTS = \
"CMakeFiles/COORD_calc.dir/coordcalc.cpp.o"

# External object files for target COORD_calc
COORD_calc_EXTERNAL_OBJECTS =

COORD_calc: CMakeFiles/COORD_calc.dir/coordcalc.cpp.o
COORD_calc: CMakeFiles/COORD_calc.dir/build.make
COORD_calc: CMakeFiles/COORD_calc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable COORD_calc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/COORD_calc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/COORD_calc.dir/build: COORD_calc
.PHONY : CMakeFiles/COORD_calc.dir/build

CMakeFiles/COORD_calc.dir/requires: CMakeFiles/COORD_calc.dir/coordcalc.cpp.o.requires
.PHONY : CMakeFiles/COORD_calc.dir/requires

CMakeFiles/COORD_calc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/COORD_calc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/COORD_calc.dir/clean

CMakeFiles/COORD_calc.dir/depend:
	cd /home/user/Kezia/CoordinateCalc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Kezia/CoordinateCalc /home/user/Kezia/CoordinateCalc /home/user/Kezia/CoordinateCalc /home/user/Kezia/CoordinateCalc /home/user/Kezia/CoordinateCalc/CMakeFiles/COORD_calc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/COORD_calc.dir/depend
