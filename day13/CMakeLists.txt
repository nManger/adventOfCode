# CMakeList.txt : CMake project for day13, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day13")

# Add source to this project's executable.
add_executable (day13 "day13.cpp" "day13.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day13 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
