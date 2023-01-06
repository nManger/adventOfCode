// day16.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <queue>

#include <regex>
#include <limits>
#include <numeric>
#include <format>

struct CaveExploration
{
	//adjacency list for the cave network: store a list of all neighbours for each cave, represent each neighbour as tuple (nodeID,distance) where nodeID == cave index in outer vector
	std::vector<std::vector<std::tuple<int,int>>> caveNetwork; 

	//list of flow rates for each cave using nodeID as list index
	std::vector<int> flowRates;
};