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
include examples/CMakeFiles/ex_premulalpha.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/ex_premulalpha.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/ex_premulalpha.dir/flags.make

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o: examples/CMakeFiles/ex_premulalpha.dir/flags.make
examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o: ../examples/ex_premulalpha.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/anhong/Downloads/allegro/b/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o   -c /home/anhong/Downloads/allegro/examples/ex_premulalpha.c

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.i"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/anhong/Downloads/allegro/examples/ex_premulalpha.c > CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.i

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.s"
	cd /home/anhong/Downloads/allegro/b/examples && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/anhong/Downloads/allegro/examples/ex_premulalpha.c -o CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.s

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.requires:
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.requires

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.provides: examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.requires
	$(MAKE) -f examples/CMakeFiles/ex_premulalpha.dir/build.make examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.provides.build
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.provides

examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.provides.build: examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o

# Object files for target ex_premulalpha
ex_premulalpha_OBJECTS = \
"CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o"

# External object files for target ex_premulalpha
ex_premulalpha_EXTERNAL_OBJECTS =

examples/ex_premulalpha: examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o
examples/ex_premulalpha: examples/CMakeFiles/ex_premulalpha.dir/build.make
examples/ex_premulalpha: lib/liballegro_image.so.5.0.10
examples/ex_premulalpha: lib/liballegro_font.so.5.0.10
examples/ex_premulalpha: lib/liballegro.so.5.0.10
examples/ex_premulalpha: lib/liballegro_main.so.5.0.10
examples/ex_premulalpha: lib/liballegro_dialog.so.5.0.10
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libpng.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libz.so
examples/ex_premulalpha: lib/liballegro.so.5.0.10
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libSM.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libICE.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libX11.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libXext.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libGL.so
examples/ex_premulalpha: /usr/lib/i386-linux-gnu/libGLU.so
examples/ex_premulalpha: examples/CMakeFiles/ex_premulalpha.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ex_premulalpha"
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex_premulalpha.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/ex_premulalpha.dir/build: examples/ex_premulalpha
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/build

examples/CMakeFiles/ex_premulalpha.dir/requires: examples/CMakeFiles/ex_premulalpha.dir/ex_premulalpha.c.o.requires
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/requires

examples/CMakeFiles/ex_premulalpha.dir/clean:
	cd /home/anhong/Downloads/allegro/b/examples && $(CMAKE_COMMAND) -P CMakeFiles/ex_premulalpha.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/clean

examples/CMakeFiles/ex_premulalpha.dir/depend:
	cd /home/anhong/Downloads/allegro/b && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anhong/Downloads/allegro /home/anhong/Downloads/allegro/examples /home/anhong/Downloads/allegro/b /home/anhong/Downloads/allegro/b/examples /home/anhong/Downloads/allegro/b/examples/CMakeFiles/ex_premulalpha.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/ex_premulalpha.dir/depend

