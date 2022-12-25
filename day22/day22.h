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

//pre-compiled vector of all cube side connections
static const std::vector<std::vector<Side>> cubeSideEdges = 
{
	{Right,Front,Left,Back}, {Left,Back,Right,Front}, //Top, Bottom
	{Right,Bottom,Left,Top}, {Left,Bottom,Right,Top}, //Front, Back
	{Front,Bottom,Back,Top}, {Back,Bottom,Left,Top}   //Left, Right
};

struct Cube
{
	std::vector<std::string> map; //map of cube net
	std::array<std::tuple<std::vector<std::string>>, 6> cubeFaces; //sub-maps of cube faces (index = Side)
	std::array<Cardinal,6> faceUpSideDirection; //which global direction the top of the sub-map is pointing to (index = Side)
	std::array<std::array<Cardinal, 4>, 6> edgeDirectionChanges; // new heading on face transfer (outer array index = Side, inner array index = Cardinal)
	std::array<std::tuple<int, int>, 6> cubeFaceCoordinateOffset; //upper left coordinate of cube face on map (index = Side)


};