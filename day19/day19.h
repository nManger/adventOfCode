// day19.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <array>
#include <set>

#include <regex>
#include <algorithm>

template<class T>
using reverseSet = std::set<T, std::greater<T>>;

enum class BotTypes
{
	NO_BOT=-1,ORE_BOT, CLAY_BOT, OBSIDIAN_BOT, GEODE_BOT
};

//stores cost of each type of robot in array in order ore,clay,obsidian
struct Blueprint
{
	int id = 0;
	int maxGeodes = 0;
	std::array<int, 3> oreBot = { 0,0,0 };
	std::array<int, 3> clayBot = { 0,0,0 };
	std::array<int, 3> obsidianBot = { 0,0,0 };
	std::array<int, 3> geodeBot = { 0,0,0 };
	std::array<int, 3> maxBots = { 0,0,0 };

	friend std::ostream& operator<< (std::ostream& stream, const Blueprint& blueprint);
};

std::ostream& operator<< (std::ostream& stream, const Blueprint& blueprint)
{
	stream << "Blueprint ID: " << blueprint.id << "\n";
	stream << "Cost ore bot: " << blueprint.oreBot[0] << "," << blueprint.oreBot[1] << "," << blueprint.oreBot[2] << " ";
	stream << "Cost clay bot: " << blueprint.clayBot[0] << "," << blueprint.clayBot[1] << "," << blueprint.clayBot[2] << " ";
	stream << "Cost obsidian bot: " << blueprint.obsidianBot[0] << "," << blueprint.obsidianBot[1] << "," << blueprint.obsidianBot[2] << " ";
	stream << "Cost geode bot: " << blueprint.geodeBot[0] << "," << blueprint.geodeBot[1] << "," << blueprint.geodeBot[2] << " ";

	return stream;
}

struct State
{
	int timer = 0;
	int maxGeodes = 0;
	BotTypes builtBot = BotTypes::ORE_BOT;
	reverseSet<BotTypes> consideredBots = {};
	std::array<int, 4> resources = { 0,0,0,0 };
	std::array<int, 4> numBots = { 1,0,0,0 };
};