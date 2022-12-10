# CMakeList.txt : CMake project for day10, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day10")

# Add source to this project's executable.
add_executable (day10 "day10.cpp" "day10.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day10 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
