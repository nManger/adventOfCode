// day22.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <variant>
#include <set>
#include <queue>

#include <regex>
#include <type_traits>
#include <numeric>

enum Cardinal { right, down, left, up, CARDINAL_SIZE };
enum Side { Top, Bottom, Front, Back, Left, Right };



struct Cube
{
	//map of cube net
	std::vector<std::string> map = std::vector<std::string>(); 

	//sub-maps of cube faces (global coordinates) (index = Side)
	std::array<std::vector<std::string>, 6> cubeFaces = std::array<std::vector<std::string>, 6>(); 

	//cardinal index offset when converting local to global heading
	std::array<int, 6> localToGlobalRotationOffset = {0,0,0,0,0,0};

	// new global face and heading on face transfer (outer array index = Side, inner array index = global Cardinal)
	std::array<std::array<std::tuple<Side,Cardinal>, 4>, 6> edgeDirectionChanges = std::array<std::array<std::tuple<Side,Cardinal>, 4>, 6>();

	//upper left coordinate of cube face on map (index = Side)
	std::array<std::tuple<int, int>, 6> cubeFaceCoordinateOffset = std::array<std::tuple<int, int>, 6>(); 

};