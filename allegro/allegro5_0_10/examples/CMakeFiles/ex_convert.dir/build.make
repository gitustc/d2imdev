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
include examples/CMakeFiles/ex_convert.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/ex_convert.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/ex_convert.dir/flags.make

examples/CMakeFiles/ex_convert.dir/ex_convert.c.o: examples/CMakeFiles/ex_convert.dir/flags.make
examples/CMakeFiles/ex_convert.dir/ex_convert.c.o: ../examples/ex_convert.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/anhong/Downloads/allegro/b/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/ex_convert.dir/ex_convert.c.o"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ex_convert.dir/ex_convert.c.o   -c /home/anhong/Downloads/allegro/examples/ex_convert.c

examples/CMakeFiles/ex_convert.dir/ex_convert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex_convert.dir/ex_convert.c.i"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/anhong/Downloads/allegro/examples/ex_convert.c > CMakeFiles/ex_convert.dir/ex_convert.c.i

examples/CMakeFiles/ex_convert.dir/ex_convert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex_convert.dir/ex_convert.c.s"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/anhong/Downloads/allegro/examples/ex_convert.c -o CMakeFiles/ex_convert.dir/ex_convert.c.s

examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.requires:
.PHONY : examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.requires

examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.provides: examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.requires
	$(MAKE) -f examples/CMakeFiles/ex_convert.dir/build.make examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.provides.build
.PHONY : examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.provides

examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.provides.build: examples/CMakeFiles/ex_convert.dir/ex_convert.c.o

# Object files for target ex_convert
ex_convert_OBJECTS = \
"CMakeFiles/ex_convert.dir/ex_convert.c.o"

# External object files for target ex_convert
ex_convert_EXTERNAL_OBJECTS =

examples/ex_convert: examples/CMakeFiles/ex_convert.dir/ex_convert.c.o
examples/ex_convert: examples/CMakeFiles/ex_convert.dir/build.make
examples/ex_convert: lib/liballegro_image.so.5.0.10
examples/ex_convert: lib/liballegro.so.5.0.10
examples/ex_convert: lib/liballegro_main.so.5.0.10
examples/ex_convert: lib/liballegro_dialog.so.5.0.10
examples/ex_convert: /usr/lib/i386-linux-gnu/libpng.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libz.so
examples/ex_convert: lib/liballegro.so.5.0.10
examples/ex_convert: /usr/lib/i386-linux-gnu/libSM.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libICE.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libX11.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libXext.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libGL.so
examples/ex_convert: /usr/lib/i386-linux-gnu/libGLU.so
examples/ex_convert: examples/CMakeFiles/ex_convert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ex_convert"
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex_convert.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/ex_convert.dir/build: examples/ex_convert
.PHONY : examples/CMakeFiles/ex_convert.dir/build

examples/CMakeFiles/ex_convert.dir/requires: examples/CMakeFiles/ex_convert.dir/ex_convert.c.o.requires
.PHONY : examples/CMakeFiles/ex_convert.dir/requires

examples/CMakeFiles/ex_convert.dir/clean:
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -P CMakeFiles/ex_convert.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/ex_convert.dir/clean

examples/CMakeFiles/ex_convert.dir/depend:
	cd /home/anhong/Downloads/allegro/b && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anhong/Downloads/allegro /home/anhong/Downloads/allegro/examples /home/anhong/Downloads/allegro/b /home/anhong/Downloads/allegro/b/examples /home/anhong/Downloads/allegro/b/examples/CMakeFiles/ex_convert.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/ex_convert.dir/depend

