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

//pre-compiled vector of all cube side connections (local coordinates)
static const std::vector<std::vector<Side>> cubeSideEdges = 
{
	{Right,Front,Left,Back}, {Right,Back,Left,Front}, //Top, Bottom
	{Right,Bottom,Left,Top}, {Left,Bottom,Right,Top}, //Front, Back
	{Front,Bottom,Back,Top}, {Back,Bottom,Front,Top}   //Left, Right
};

struct Cube
{
	std::array<int, 6> localToGlobalRotationOffset = {0,0,0,0,0,0}; //cardinal rotation offset when converting local up to global up
	std::vector<std::string> map = std::vector<std::string>(); //map of cube net
	std::array<std::vector<std::string>, 6> cubeFaces = std::array<std::vector<std::string>, 6>(); //sub-maps of cube faces (global coordinates) (index = Side)
	std::array<Cardinal,6> faceUpSideDirection = std::array<Cardinal, 6>(); //which global direction the local top direction of the sub-map is pointing to (index = Side)
	std::array<std::array<Cardinal, 4>, 6> edgeDirectionChanges = std::array<std::array<Cardinal, 4>, 6>(); // new global heading on face transfer (outer array index = Side, inner array index = Cardinal)
	std::array<std::tuple<int, int>, 6> cubeFaceCoordinateOffset = std::array<std::tuple<int, int>, 6>(); //upper left coordinate of cube face on map (index = Side)

};