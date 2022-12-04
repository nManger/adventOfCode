// day4.cpp : Defines the entry point for the application.
//

#include "day4.h"

using namespace std;

vector<vector<int>> preparePuzzleInput(string filename)
{
	ifstream myfile(filename);
	vector<vector<int>> puzzleInput;
	regex splitTokens{ R"([-,])" };

	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			vector<int> pieces;
			sregex_token_iterator it(line.begin(), line.end(), splitTokens, -1);
			sregex_token_iterator end; 
			for ( ; it != end; it++)
			{
				pieces.push_back(stoi(it->str()));
			}
			puzzleInput.push_back(pieces);
		}
		myfile.close();
	}
	return puzzleInput;
}

int findNumFullyContainedAssignments(vector<vector<int>> assignmentList)
{
	int num = 0;

	for (vector<int> assignment : assignmentList)
	{
		if ((assignment[0] <= assignment[2] && assignment[1] >= assignment[3]) ||	//assignment 1 contains assignment 2
			(assignment[0] >= assignment[2] && assignment[1] <= assignment[3]))		//assignment 2 contains assignment 1
		{
			num++;
		}
	}
	return num;
}

int findNumAssignmentsWOverlap(vector<vector<int>> assignmentList)
{
	int num = 0;

	for (vector<int> assignment : assignmentList)
	{
		//check for assignments with no overlap and negate
		if (!((assignment[1] < assignment[2] ) ||	//assignment 1 before assignment 2
			(assignment[3] < assignment[0] )))		//assignment 2 before assignment 1
		{
			num++;
		}
	}
	return num;
}

int main()
{
	vector<vector<int>> testInput = preparePuzzleInput("puzzleTest.txt");
	vector<vector<int>> mainInput = preparePuzzleInput("puzzleInput.txt");
	
	cout << "Part 1:\n";
	cout << "Number of pairs with fully contained assignments (test puzzle): " << findNumFullyContainedAssignments(testInput) << ".\n";
	cout << "Number of pairs with fully contained assignments (main puzzle): " << findNumFullyContainedAssignments(mainInput) << ".\n";
	
	cout << "\n Part 2:\n";
	cout << "Number of pairs with fully contained assignments (test puzzle): " << findNumAssignmentsWOverlap(testInput) << ".\n";
	cout << "Number of pairs with fully contained assignments (main puzzle): " << findNumAssignmentsWOverlap(mainInput) << ".\n";

	return 0;
}
