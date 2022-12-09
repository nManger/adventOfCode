# CMakeList.txt : CMake project for day9, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day9")

# Add source to this project's executable.
add_executable (day9 "day9.cpp" "day9.h" "bridge.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day9 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
