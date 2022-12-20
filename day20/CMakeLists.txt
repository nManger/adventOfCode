# CMakeList.txt : CMake project for day20, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day20")

# Add source to this project's executable.
add_executable (day20 "day20.cpp" "day20.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day20 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
