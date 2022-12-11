// day11.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <tuple>
#include <queue>
#include <string>

#include <regex>
#include <cstdlib>
#include <functional>

static long LCM_ALL_MONKEYS;

template<typename T>
class Monkey
{
	std::queue<T> itemsHeld;
	std::function<T(T)> operation;
	T testDivisor=1;
	int monkeyTrue=0;
	int monkeyFalse=0;
	T numItemsInspected =0;

public:
	
	Monkey(const std::vector<T>& items,std::function<T(T)> operation, T testDivisor, int monkeyTrue, int monkeyFalse);
	Monkey(const Monkey&);
	bool hasItems();
	std::tuple<int, T> inspectNextItem();
	void receiveItem(T item);
	T getNumItemsInspected();
};


template<typename T>
class MonkeyGame
{
	std::vector<Monkey<T>> monkeyList;
	

public:
	MonkeyGame(std::string filename);
	T playGame(int turns);
};