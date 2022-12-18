// day16.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <tuple>
#include <map>

#include <regex>
#include <limits>

struct CaveExploration
{
	std::vector<std::vector<std::tuple<int,int>>> caveNetwork;
	std::vector<int> flowRates;
};