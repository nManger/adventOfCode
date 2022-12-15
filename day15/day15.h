// day15.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <tuple>
#include <set>

#include <cstdlib>
#include <regex>


struct Sensor{
	std::tuple<int,int> position;
	std::tuple<int,int> nearestBeacon;
	
	int distanceSensorBeacon;
};