// day21.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <string>
#include <tuple>

#include <cstdlib>
#include <regex>
#include <format>

struct Monkey
{
	std::string name = "";
	char operation = '+';
	int number = 0;
	std::string childA = "";
	std::string childB = "";
};

struct MonkeyGraph
{
	std::vector<Monkey> monkeyList;
	std::map<std::string, int> monkeyLookup;
	std::vector<std::vector<int>> monkeyAdjList;

	void writeMonkeyGraph();
};
