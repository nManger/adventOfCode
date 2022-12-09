// day9.cpp : Defines the entry point for the application.
//

#include "day9.h"

using namespace std;

vector<tuple<char, int>> readPuzzleInput(string filename)
{
	vector<tuple<char, int>> directives;

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			directives.push_back(make_tuple(line[0],stoi(line.substr(2))));
		}
		myfile.close();
	}
	return directives;
}

int simulateRopeMovement(vector<tuple<char,int>> directives,int nknots)
{
	Rope myRope(nknots);

	for (auto [direction, steps] : directives)
	{
		myRope.move(direction, steps);
	}

	//myRope.plotTailMovement(6);

	return myRope.getPrevTailPos().size();
}

int main()
{
	vector<tuple<char, int>> testDirectives = readPuzzleInput("puzzleTest.txt");
	vector<tuple<char, int>> puzzleDirectives = readPuzzleInput("puzzleInput.txt");

	int i;

	cout << "Part 1:\n\n";
	i = simulateRopeMovement(testDirectives,2);
	cout << "Number of unique positions visited (test): " << i << ".\n\n";
	i = simulateRopeMovement(puzzleDirectives, 2);
	cout << "Number of unique positions visited (puzzle): " << i << ".\n\n";

	cout << "Part 2:\n\n";
	testDirectives = readPuzzleInput("puzzleTest2.txt");
	i = simulateRopeMovement(testDirectives, 10);
	cout << "Number of unique positions visited (test): " << i << ".\n\n";
	i = simulateRopeMovement(puzzleDirectives, 10);
	cout << "Number of unique positions visited (puzzle): " << i << ".\n";

}
