# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build

# Include any dependencies generated for this target.
include CMakeFiles/CG_P5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CG_P5.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CG_P5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CG_P5.dir/flags.make

CMakeFiles/CG_P5.dir/main.cpp.o: CMakeFiles/CG_P5.dir/flags.make
CMakeFiles/CG_P5.dir/main.cpp.o: /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/main.cpp
CMakeFiles/CG_P5.dir/main.cpp.o: CMakeFiles/CG_P5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CG_P5.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CG_P5.dir/main.cpp.o -MF CMakeFiles/CG_P5.dir/main.cpp.o.d -o CMakeFiles/CG_P5.dir/main.cpp.o -c /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/main.cpp

CMakeFiles/CG_P5.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CG_P5.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/main.cpp > CMakeFiles/CG_P5.dir/main.cpp.i

CMakeFiles/CG_P5.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CG_P5.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/main.cpp -o CMakeFiles/CG_P5.dir/main.cpp.s

# Object files for target CG_P5
CG_P5_OBJECTS = \
"CMakeFiles/CG_P5.dir/main.cpp.o"

# External object files for target CG_P5
CG_P5_EXTERNAL_OBJECTS =

CG_P5: CMakeFiles/CG_P5.dir/main.cpp.o
CG_P5: CMakeFiles/CG_P5.dir/build.make
CG_P5: CMakeFiles/CG_P5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CG_P5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CG_P5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CG_P5.dir/build: CG_P5
.PHONY : CMakeFiles/CG_P5.dir/build

CMakeFiles/CG_P5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CG_P5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CG_P5.dir/clean

CMakeFiles/CG_P5.dir/depend:
	cd /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build /home/sasha/Desktop/My_CG/P6/P6_Surface/Ruled_Surface/build/CMakeFiles/CG_P5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CG_P5.dir/depend

