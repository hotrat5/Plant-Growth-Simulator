# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /mnt/hgfs/lvgl/lvgl-arm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/lvgl/lvgl-arm/build

# Include any dependencies generated for this target.
include CMakeFiles/lvgl-arm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lvgl-arm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lvgl-arm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lvgl-arm.dir/flags.make

CMakeFiles/lvgl-arm.dir/main.c.o: CMakeFiles/lvgl-arm.dir/flags.make
CMakeFiles/lvgl-arm.dir/main.c.o: ../main.c
CMakeFiles/lvgl-arm.dir/main.c.o: CMakeFiles/lvgl-arm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/lvgl/lvgl-arm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lvgl-arm.dir/main.c.o"
	/usr/local/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lvgl-arm.dir/main.c.o -MF CMakeFiles/lvgl-arm.dir/main.c.o.d -o CMakeFiles/lvgl-arm.dir/main.c.o -c /mnt/hgfs/lvgl/lvgl-arm/main.c

CMakeFiles/lvgl-arm.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lvgl-arm.dir/main.c.i"
	/usr/local/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/hgfs/lvgl/lvgl-arm/main.c > CMakeFiles/lvgl-arm.dir/main.c.i

CMakeFiles/lvgl-arm.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lvgl-arm.dir/main.c.s"
	/usr/local/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/hgfs/lvgl/lvgl-arm/main.c -o CMakeFiles/lvgl-arm.dir/main.c.s

# Object files for target lvgl-arm
lvgl__arm_OBJECTS = \
"CMakeFiles/lvgl-arm.dir/main.c.o"

# External object files for target lvgl-arm
lvgl__arm_EXTERNAL_OBJECTS =

lvgl-arm: CMakeFiles/lvgl-arm.dir/main.c.o
lvgl-arm: CMakeFiles/lvgl-arm.dir/build.make
lvgl-arm: lib/liblvgl.a
lvgl-arm: lvgl/liblvgl_examples.a
lvgl-arm: lvgl/liblvgl_demos.a
lvgl-arm: lib/liblv_drivers.a
lvgl-arm: UI/libui.a
lvgl-arm: lib/liblvgl.a
lvgl-arm: CMakeFiles/lvgl-arm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/hgfs/lvgl/lvgl-arm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable lvgl-arm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lvgl-arm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lvgl-arm.dir/build: lvgl-arm
.PHONY : CMakeFiles/lvgl-arm.dir/build

CMakeFiles/lvgl-arm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lvgl-arm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lvgl-arm.dir/clean

CMakeFiles/lvgl-arm.dir/depend:
	cd /mnt/hgfs/lvgl/lvgl-arm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/lvgl/lvgl-arm /mnt/hgfs/lvgl/lvgl-arm /mnt/hgfs/lvgl/lvgl-arm/build /mnt/hgfs/lvgl/lvgl-arm/build /mnt/hgfs/lvgl/lvgl-arm/build/CMakeFiles/lvgl-arm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lvgl-arm.dir/depend

