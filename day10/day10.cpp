// day10.cpp : Defines the entry point for the application.
//

#include "day10.h"

using namespace std;


/*
* Read puzzle input from file.
* Decodes noop to 0 and addx to 1.
*/
vector<tuple<int, int>> readPuzzleInput(string filename)
{
	vector<tuple<int, int>> instructions;
	ifstream myfile(filename);

	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			size_t spacePos = line.find(' ');

			//noop, set second arg (arbitrarily) to 0
			if (spacePos == string::npos)
			{
				instructions.push_back({0,0});
			}
			else
			{
				instructions.push_back({ 1,stoi(line.substr(spacePos + 1)) });
			}
		}
		myfile.close();
	}
	else
	{
		cout << "Could not open file" << filename << ".\n";
	}
	return instructions;
}

int getCombinedSignalStrength(const vector<tuple<int, int>>& instructions)
{
	//create program queue from instructions
	queue<tuple<int,int>> program;
	for (auto& step : instructions)
	{
		program.push(step);
	}


	bool stepTwo = false;
	int X = 1;
	int cycle = 1;
	int saveCycle = 20;
	int saveCycleInterval = 40;
	int signalStrengthCombined = 0;
	int directive=0, V=0;

	while (!program.empty())
	{
		if (cycle == saveCycle)
		{
			signalStrengthCombined += cycle * X;
			saveCycle += saveCycleInterval;
			//cout << "Cycle: " << cycle << " SignalStrenght: " << (cycle * X) << "\n";
		}

		//we are in step 2 of an add directive
		if (stepTwo)
		{
			X += V;
			stepTwo = false;
		}
		else
		{
			//get next directive
			tie(directive, V) = program.front();
			program.pop();

			//we are in an add directive, need second step
			if (directive == 1)
			{
				stepTwo = true;
			}
			//else we are in noop directive, nothing to be done
		}

		cycle += 1;
	}
	return signalStrengthCombined;
}

void writeCRTImage(const vector<tuple<int, int>>& instructions, int screenWidth)
{
	//create program queue from instructions
	queue<tuple<int, int>> program;
	for (auto& step : instructions)
	{
		program.push(step);
	}


	bool stepTwo = false;
	int X = 1; //sprite pos
	int cycle = 0;
	int directive = 0, V = 0;

	while (!program.empty())
	{

		//check if we need to write newline char
		if (cycle%screenWidth == 0)
		{
			cout << "\n";
		}

		//write CRT pixel
		//sprite overlaps with CRT pixel 
		if (abs((cycle % screenWidth) - X) <= 1)
		{
			cout << "#";
		}
		else
		{
			cout << ".";
		}

		//we are in step 2 of an add directive
		if (stepTwo)
		{
			X += V;
			stepTwo = false;
		}
		else
		{
			//get next directive
			tie(directive, V) = program.front();
			program.pop();

			//we are in an add directive, need second step
			if (directive == 1)
			{
				stepTwo = true;
			}
			//else we are in noop directive, nothing to be done
		}

		cycle += 1;
	}
	cout << "\n\n";
}

int main()
{
	vector<tuple<int, int>> testDirectives = readPuzzleInput("puzzleTest.txt");
	vector<tuple<int, int>> puzzleDirectives = readPuzzleInput("puzzleInput.txt");

	cout << "Part 1:\n";
	cout << "Combined signal strength from test directives: " << getCombinedSignalStrength(testDirectives) << "\n";
	cout << "Combined signal strength from test directives: " << getCombinedSignalStrength(puzzleDirectives) << "\n";

	cout << "\n Part 1:\n";
	cout << "The image drawn by the test program is:\n";
	writeCRTImage(testDirectives,40);
	cout << "The image drawn by the puzzle program is:\n";
	writeCRTImage(puzzleDirectives, 40);

	return 0;
}
