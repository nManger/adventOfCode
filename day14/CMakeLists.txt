# CMakeList.txt : CMake project for day14, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day14")

# Add source to this project's executable.
add_executable (day14 "day14.cpp" "day14.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day14 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
