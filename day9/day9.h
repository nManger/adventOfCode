// day9.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <string>
#include <set>
#include <vector>


class Rope
{
	std::vector<std::tuple<int, int>> knotPos;
	std::set<std::tuple<int, int>> prevTailPos;
	const int numKnots;

public:
	Rope();
	Rope(int nKnots);
	Rope(int nKnots,const std::tuple<int, int>& startPos);
	void move(char direction, int steps);
	std::set<std::tuple<int, int>> getPrevTailPos();
	void plotTailMovement(int minGridSize);
	void plotRopePosition();

private:
	void moveHead(char direction);
	void adjustTail();
	std::vector<std::vector<char>> generatePointStack(std::set<std::tuple<int, int>> points,int minGridSize);
	int  sign(int x) { return x < 0 ? -1 : 1; }
};