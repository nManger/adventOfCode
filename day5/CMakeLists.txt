# CMakeList.txt : CMake project for day5, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("day5")

# Add source to this project's executable.
add_executable (day5 "day5.cpp" "day5.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET day5 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
file(COPY ${CMAKE_SOURCE_DIR}/puzzleInput.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/puzzleTest.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})