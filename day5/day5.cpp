// day5.cpp : Defines the entry point for the application.
//

#include "day5.h"

using namespace std;

tuple<vector<vector<char>>, vector<vector<int>>> preparePuzzleInput(string filename)
{
	ifstream myfile(filename);
	vector<vector<char>> crateStack;
	vector<vector<int>> moveDirectives;

	if (myfile.is_open())
	{
		vector<string> lines;
		
		string line;
		//read part one of input file
		while (getline(myfile, line))
		{
			//part one delimited by empty line
			if (line.empty()) break;
			lines.push_back(line);
		}

		string stackNumbers = lines.back();
		lines.pop_back();
		size_t lastStackNumber = stackNumbers.find_last_not_of(" \t\f\n\r");
		int stackSize = stackNumbers[lastStackNumber] - '0';
		for (int i = 0; i < stackSize; i++)
		{
			vector<char> dummy;
			crateStack.push_back(dummy);
		}

		for (vector<string>::reverse_iterator rit = lines.rbegin(); rit != lines.rend(); rit++)
		{
			string line = *rit;
			//line[0] is always guaranteed to be space or [, so start at 1
			for (int i = 1; i < line.size(); i++)
			{
				if (isalpha(line[i]))
				{
					int j = (i - 1) / 4;
					crateStack[j].push_back(line[i]);
				}
			}
		}

		regex reg{ R"([ ])" };
		//read part two of input file and process
		while (getline(myfile, line))
		{
			sregex_token_iterator it(line.begin(), line.end(),reg, -1);
			sregex_token_iterator end;
			vector<int> lineDirective;

			//regex split line by space characters and filter for numbers
			for (; it != end; it++)
			{
				//if first char is digit, string contains a number
				if (isdigit(it->str()[0]))
				{
					lineDirective.push_back(stoi(it->str()));
				}
			}
			moveDirectives.push_back(lineDirective);
		}

		myfile.close();
	}

	return { crateStack, moveDirectives};
}


/*
Re-arrange the crates in crateStack in-place using the given directives and part 1 instructions.
*/
void rearrangeCrates(vector<vector<char>>& crateStack, const vector<vector<int>>& directives)
{
	int iTower1, iTower2;

	for (vector<int> directive : directives)
	{
		//tower numbers in directives are one-based, correct -1 to 0-based indexing
		iTower1 = directive[1] - 1;
		iTower2 = directive[2] - 1;

		for (int i = 0; i < directive[0]; i++)
		{
			crateStack[iTower2].push_back(crateStack[iTower1].back());
			crateStack[iTower1].pop_back();
		}
	}
}

/*
Re-arrange the crates in crateStack in-place using the given directives and part 2 instructions.
*/
void rearrangeCratesPart2(vector<vector<char>>& crateStack, const vector<vector<int>>& directives)
{
	int iTower1, iTower2;

	for (vector<int> directive : directives)
	{
		//tower numbers in directives are one-based, correct -1 to 0-based indexing
		iTower1 = directive[1] - 1;
		iTower2 = directive[2] - 1;

		crateStack[iTower2].insert(crateStack[iTower2].end(),crateStack[iTower1].end()-directive[0], crateStack[iTower1].end());
		crateStack[iTower1].erase(crateStack[iTower1].end() - directive[0], crateStack[iTower1].end());
	}
}

string determineTopCrates(const vector<vector<char>>& crateStack)
{
	string topCrates = "";
	for (vector<char> tower : crateStack)
	{
		topCrates.push_back(tower.back());
	}
	return topCrates;
}

vector<vector<char>> deepCopyCrateStack(const vector<vector<char>>& crateStack)
{
	vector<vector<char>> copyCrateStack;
	for (vector<char> tower : crateStack)
	{
		vector<char> copyTower(tower);
		copyCrateStack.push_back(copyTower);
	}
	return copyCrateStack;
}

int main()
{
	auto [testStack, testDirectives] = preparePuzzleInput("puzzleTest.txt");
	auto [puzzleStack, puzzleDirectives] = preparePuzzleInput("puzzleInput.txt");
	
	auto testStack2 = deepCopyCrateStack(testStack);
	auto puzzleStack2 = deepCopyCrateStack(puzzleStack);

	rearrangeCrates(testStack, testDirectives);
	rearrangeCrates(puzzleStack, puzzleDirectives);

	cout << "Part 1:\n";
	cout << "The top crates in the test stack after re-arrangement are: " << determineTopCrates(testStack) << ".\n";
	cout << "The top crates in the puzzle stack after re-arrangement are: " << determineTopCrates(puzzleStack) << ".\n";
	
	rearrangeCratesPart2(testStack2, testDirectives);
	rearrangeCratesPart2(puzzleStack2, puzzleDirectives);

	cout << "Part 2:\n";
	cout << "The top crates in the test stack after re-arrangement are: " << determineTopCrates(testStack2) << ".\n";
	cout << "The top crates in the puzzle stack after re-arrangement are: " << determineTopCrates(puzzleStack2) << ".\n";

	return 0;
}
