# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\genna\OneDrive\Desktop\darbs\branches\xml

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\build

# Include any dependencies generated for this target.
include ../util/build/CMakeFiles/darbsutil.dir/depend.make

# Include the progress variables for this target.
include ../util/build/CMakeFiles/darbsutil.dir/progress.make

# Include the compile flags for this target's objects.
include ../util/build/CMakeFiles/darbsutil.dir/flags.make

../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.obj: ../util/build/CMakeFiles/darbsutil.dir/flags.make
../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.obj: ../util/build/CMakeFiles/darbsutil.dir/includes_CXX.rsp
../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.obj: ../util/src/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.obj"
	cd /d C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build && C:\Users\genna\OneDrive\Desktop\JOB\DARBSenv\mingw\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\darbsutil.dir\src\util.cpp.obj -c C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\src\util.cpp

../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/darbsutil.dir/src/util.cpp.i"
	cd /d C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build && C:\Users\genna\OneDrive\Desktop\JOB\DARBSenv\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\src\util.cpp > CMakeFiles\darbsutil.dir\src\util.cpp.i

../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/darbsutil.dir/src/util.cpp.s"
	cd /d C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build && C:\Users\genna\OneDrive\Desktop\JOB\DARBSenv\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\src\util.cpp -o CMakeFiles\darbsutil.dir\src\util.cpp.s

# Object files for target darbsutil
darbsutil_OBJECTS = \
"CMakeFiles/darbsutil.dir/src/util.cpp.obj"

# External object files for target darbsutil
darbsutil_EXTERNAL_OBJECTS =

../util/build/libdarbsutil.dll: ../util/build/CMakeFiles/darbsutil.dir/src/util.cpp.obj
../util/build/libdarbsutil.dll: ../util/build/CMakeFiles/darbsutil.dir/build.make
../util/build/libdarbsutil.dll: ../util/build/CMakeFiles/darbsutil.dir/linklibs.rsp
../util/build/libdarbsutil.dll: ../util/build/CMakeFiles/darbsutil.dir/objects1.rsp
../util/build/libdarbsutil.dll: ../util/build/CMakeFiles/darbsutil.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libdarbsutil.dll"
	cd /d C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\darbsutil.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
../util/build/CMakeFiles/darbsutil.dir/build: ../util/build/libdarbsutil.dll

.PHONY : ../util/build/CMakeFiles/darbsutil.dir/build

../util/build/CMakeFiles/darbsutil.dir/clean:
	cd /d C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build && $(CMAKE_COMMAND) -P CMakeFiles\darbsutil.dir\cmake_clean.cmake
.PHONY : ../util/build/CMakeFiles/darbsutil.dir/clean

../util/build/CMakeFiles/darbsutil.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\genna\OneDrive\Desktop\darbs\branches\xml C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\build C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build C:\Users\genna\OneDrive\Desktop\darbs\branches\xml\util\build\CMakeFiles\darbsutil.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : ../util/build/CMakeFiles/darbsutil.dir/depend

