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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anhong/Downloads/allegro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anhong/Downloads/allegro/b

# Include any dependencies generated for this target.
include examples/CMakeFiles/ex_pixelformat.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/ex_pixelformat.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/ex_pixelformat.dir/flags.make

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o: examples/CMakeFiles/ex_pixelformat.dir/flags.make
examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o: ../examples/ex_pixelformat.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/anhong/Downloads/allegro/b/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o -c /home/anhong/Downloads/allegro/examples/ex_pixelformat.cpp

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.i"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/anhong/Downloads/allegro/examples/ex_pixelformat.cpp > CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.i

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.s"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/anhong/Downloads/allegro/examples/ex_pixelformat.cpp -o CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.s

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.requires:
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.requires

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.provides: examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/ex_pixelformat.dir/build.make examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.provides.build
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.provides

examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.provides.build: examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o: examples/CMakeFiles/ex_pixelformat.dir/flags.make
examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o: ../examples/nihgui.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/anhong/Downloads/allegro/b/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o -c /home/anhong/Downloads/allegro/examples/nihgui.cpp

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex_pixelformat.dir/nihgui.cpp.i"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/anhong/Downloads/allegro/examples/nihgui.cpp > CMakeFiles/ex_pixelformat.dir/nihgui.cpp.i

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex_pixelformat.dir/nihgui.cpp.s"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/anhong/Downloads/allegro/examples/nihgui.cpp -o CMakeFiles/ex_pixelformat.dir/nihgui.cpp.s

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.requires:
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.requires

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.provides: examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/ex_pixelformat.dir/build.make examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.provides.build
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.provides

examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.provides.build: examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o

# Object files for target ex_pixelformat
ex_pixelformat_OBJECTS = \
"CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o" \
"CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o"

# External object files for target ex_pixelformat
ex_pixelformat_EXTERNAL_OBJECTS =

examples/ex_pixelformat: examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o
examples/ex_pixelformat: examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o
examples/ex_pixelformat: examples/CMakeFiles/ex_pixelformat.dir/build.make
examples/ex_pixelformat: lib/liballegro_font.so.5.0.10
examples/ex_pixelformat: lib/liballegro_primitives.so.5.0.10
examples/ex_pixelformat: lib/liballegro_image.so.5.0.10
examples/ex_pixelformat: lib/liballegro.so.5.0.10
examples/ex_pixelformat: lib/liballegro_main.so.5.0.10
examples/ex_pixelformat: lib/liballegro_dialog.so.5.0.10
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libpng.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libz.so
examples/ex_pixelformat: lib/liballegro.so.5.0.10
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libSM.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libICE.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libX11.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libXext.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libGL.so
examples/ex_pixelformat: /usr/lib/i386-linux-gnu/libGLU.so
examples/ex_pixelformat: examples/CMakeFiles/ex_pixelformat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ex_pixelformat"
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex_pixelformat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/ex_pixelformat.dir/build: examples/ex_pixelformat
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/build

examples/CMakeFiles/ex_pixelformat.dir/requires: examples/CMakeFiles/ex_pixelformat.dir/ex_pixelformat.cpp.o.requires
examples/CMakeFiles/ex_pixelformat.dir/requires: examples/CMakeFiles/ex_pixelformat.dir/nihgui.cpp.o.requires
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/requires

examples/CMakeFiles/ex_pixelformat.dir/clean:
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -P CMakeFiles/ex_pixelformat.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/clean

examples/CMakeFiles/ex_pixelformat.dir/depend:
	cd /home/anhong/Downloads/allegro/b && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anhong/Downloads/allegro /home/anhong/Downloads/allegro/examples /home/anhong/Downloads/allegro/b /home/anhong/Downloads/allegro/b/examples /home/anhong/Downloads/allegro/b/examples/CMakeFiles/ex_pixelformat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/ex_pixelformat.dir/depend

