// day1.cpp : Defines the entry point for the application.
//

#include "day1.h"

using namespace std;

/*
	Function reading input text file
	Returns vector containing each elf's input as a vector
*/
vector<vector<int>> readInputFile(const string filename)
{
	string line;
	ifstream myfile(filename);
	vector<vector<int>> input;
	if (myfile.is_open())
	{
		vector<int> batch;
		//iterate over file line by line
		while (getline(myfile, line))
		{
			//each elf content is delimited by empty lines, so
			//check for empty lines to find end of batch
			if (line.empty())
			{
				input.push_back(batch);
				batch.clear();
				continue;
			}
			batch.push_back(stoi(line));
		}
		//push final batch and close file
		input.push_back(batch);
		myfile.close();
	}

	return input;
}

vector<int> calculateElfCalories(vector<vector<int>> elfCalories)
{
	vector<int> totalElfCalories;
	int caloryCount=0;

	for (int i = 0; i < elfCalories.size(); i++)
	{
		for (int j = 0; j < elfCalories[i].size(); j++)
		{
			caloryCount += elfCalories[i][j];
		}
		totalElfCalories.push_back(caloryCount);
		caloryCount = 0;
	}
	return totalElfCalories;
}

tuple<int, int> findElfWithMostCalories(vector<int> elfCalories)
{
	int mostCalories = 0;
	int elf = 0;

	for (int i = 0; i < elfCalories.size(); i++)
	{
		if (elfCalories[i] > mostCalories)
		{
			mostCalories = elfCalories[i];
			elf = i;
		}
	}
	return tuple<int, int>{elf, mostCalories};
}

vector<tuple<int,int>> findTopThreeElvesWithMostCalories(vector<int> elfCalories)
{
	int mostCalories[3] = { 0, 0, 0 };
	int elf[3] = { 0, 0, 0 };

	for (int i = 0; i < elfCalories.size(); i++)
	{
		if (elfCalories[i] > mostCalories[0])
		{
			mostCalories[2] = mostCalories[1];
			elf[2] = elf[1];

			mostCalories[1] = mostCalories[0];
			elf[1] = elf[0];

			mostCalories[0] = elfCalories[i];
			elf[0] = i;
			continue;
		}
		if (elfCalories[i] > mostCalories[1])
		{
			mostCalories[2] = mostCalories[1];
			elf[2] = elf[1];

			mostCalories[1] = elfCalories[i];
			elf[1] = i;
			continue;
		}
		if (elfCalories[i] > mostCalories[2])
		{
			mostCalories[2] = elfCalories[i];
			elf[2] = i;
		}
	}
	return vector<tuple<int, int>>{ {elf[0], mostCalories[0]}, { elf[1],mostCalories[1] }, { elf[2],mostCalories[2] }};

}

int main()
{
	vector<vector<int>> testInput = readInputFile("puzzleTest.txt");
	vector<vector<int>> puzzleInput = readInputFile("puzzleInput.txt");

	int maxCal, elf;
	
	cout << "Part 1\n";
	vector<int> elfCaloriesTest = calculateElfCalories(testInput);
	tie(elf,maxCal) = findElfWithMostCalories(elfCaloriesTest);
	cout << "Elf " << elf+1 << " has the maximum calory count " << maxCal << ".\n";

	vector<int> elfCalories = calculateElfCalories(puzzleInput);
	tie(elf, maxCal) = findElfWithMostCalories(elfCalories);
	cout << "Elf " << elf + 1 << " has the maximum calory count " << maxCal << ".\n";

	cout << "\n";
	cout << "Part 2:\n";

	vector<tuple<int, int>> topThreeElvesCalories = findTopThreeElvesWithMostCalories(elfCaloriesTest);
	cout << "The elves with top 3 calory count are:";
	maxCal = 0;
	for (auto&& [elf, cal] : topThreeElvesCalories)
	{
		cout << " " << elf+1 << " ( " << cal << " calories )";
		maxCal += cal;
	}
	cout << "\n";
	cout << "Their total calory count is " << maxCal << ".\n\n";
	
	topThreeElvesCalories = findTopThreeElvesWithMostCalories(elfCalories);
	cout << "The elves with top 3 calory count are:";
	maxCal = 0;
	for (auto&& [elf, cal] : topThreeElvesCalories)
	{
		cout << " " << elf+1 << " ( " << cal << " calories )";
		maxCal += cal;
	}
	cout << "\n";
	cout << "Their total calory count is " << maxCal << ".\n";
}
