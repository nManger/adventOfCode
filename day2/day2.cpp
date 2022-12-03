// day2.cpp : Defines the entry point for the application.
//

#include "day2.h"

using namespace std;

/*
Decode puzzle input data:

Elf 1: A = Rock, B = Paper, C = Scissor
Elf 2 (You): X = Rock, Y = Paper, Z = Scissor

Assign input scores as follows: Rock = 0, Paper = 1, Scissor = 2.

*/
vector<tuple<int, int>> preparePuzzleInput(string filename)
{
	vector<tuple<int, int>> puzzleInput;
	
	ifstream myfile(filename);

	if (myfile.is_open())
	{
		string line;
		string a, b;
		int x = 0, y = 0;
		while ( getline(myfile,line))
		{
			
			size_t pos = line.find(' ', 0);
			a = line.substr(0, pos);
			b = line.substr(pos + 1);
			
 			if (a == "A" || a == "a")
			{
				x = 0;
			}
			else if (a == "B" || a == "b")
			{
				x = 1;
			}
			else if (a == "C" || a == "c")
			{
				x = 2;
			}
			else
			{
				throw invalid_argument("First elf input not Rock, Paper or Scissor. Aborting.");
			}
			
			if (b == "X" || b == "x")
			{
				y = 0;
			}
			else if (b == "Y" || b == "y")
			{
				y = 1;
			}
			else if (b == "Z" || b == "z")
			{
				y = 2;
			}
			else
			{
				throw invalid_argument("Second elf input not Rock, Paper or Scissor. Aborting.");
			}

			puzzleInput.push_back({x,y});
		}

		myfile.close();
	}
	return puzzleInput;
}

/*
Play one game of Rock-Paper-Scissor using part 1 instructions:

Rock beats Scissor beats Paper beats Rock ...
Final score is calulated from game score (Win = 6, draw = 3, loss = 0) plus elf 2's/your input score + 1.

Input tuple is defined as  {elf1's score, elf2's/your score}.

Return score for elf 2/you
*/
int playGame(tuple<int, int> gameInput)
{
	int score = 0;

	//Calculate input score difference
	int diff = get<1>(gameInput) - get<0>(gameInput);
	
	// draw
	if (diff == 0)
	{
		score = 3;
	}
	// win elf 2/you
	else if (diff == 1 || diff == -2)
	{
		score = 6;
	}
	// else: win elf 1, nothing to do

	return score + get<1>(gameInput) + 1;
}

/*
Play one game of Rock-Paper-Scissor using part 2 instructions:

Rock beats Scissor beats Paper beats Rock ...
Final score is calulated from game score (Win = 6, draw = 3, loss = 0) plus elf 2's/your input score + 1.

Input tuple is defined as  {elf1's score, match result}.
Match result is win = 2, draw = 1, loss = 0.

Return score for elf 2/you
*/
int playGamePartTwo(tuple<int, int> gameInput)
{
	int score = get<1>(gameInput);

	int diff = 0;
	
	//draw -> chose same score as elf 1
	if (score == 1)
	{
		score = score * 3 + get<0>(gameInput) + 1;
	}
	//win -> chose (elf1score + 1) mod 3
	else if (score == 2)
	{
		score = score * 3 + (get<0>(gameInput) + 1) % 3 + 1;

	}
	//loose -> chose (elf1score - 1) mod 3
	else
	{
		// correct remainder for negative numbers for use as mod
		int r = (get<0>(gameInput) - 1) % 3;
		r = r < 0 ? r + 3 : r;

		// game score is 0
		score += r + 1;
	}
	return score ;
}

/*
Play one match of multiple rock-paper-scissor games

Use partTwo = true to play using part 2 instructions.
*/
int playMatch(vector<tuple<int, int>> matchInput,bool partTwo = false)
{
	int score = 0;
	if (partTwo)
	{
		for (tuple<int, int> gameInput : matchInput)
		{
			score += playGamePartTwo(gameInput);
		}
	}
	else
	{
		for (tuple<int, int> gameInput : matchInput)
		{
			score += playGame(gameInput);
		}
	}
	
	return score;
}

int main()
{
	vector<tuple<int, int>> puzzleTest = preparePuzzleInput("puzzleTest.txt");
	vector<tuple<int, int>> puzzleInput = preparePuzzleInput("puzzleInput.txt");
	
	cout << "Part 1: \n";
	cout << "Your final game score in the test match is " << playMatch(puzzleTest) << ".\n";
	cout << "Your final game score in the main match is " << playMatch(puzzleInput) << ".\n";
	cout << "\n";

	cout << "Part 2: \n";
	cout << "Your final game score in the test match is " << playMatch(puzzleTest,true) << ".\n";
	cout << "Your final game score in the main match is " << playMatch(puzzleInput,true) << ".\n";
	cout << "\n";

	return 0;
}
