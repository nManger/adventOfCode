// day3.cpp : Defines the entry point for the application.
//

#include "day3.h"

using namespace std;

vector<string> preparePuzzleInput(string filename)
{
	vector<string> puzzleInput;

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			puzzleInput.push_back(line);
		}
		myfile.close();
	}
	return puzzleInput;
}

char identifyItemTypeAppearingInBothCompartments(string rucksack)
{
	int compLength = rucksack.length() / 2;
	string comp1 = rucksack.substr(0, compLength);
	string comp2 = rucksack.substr(compLength);

	for (char item : comp1)
	{
		if (comp2.find(item) != string::npos)
		{
			//we can safely return here as puzzle rules guaratee that exactly one item will appear in both comps
			return item;
		}
	}
	return 0;
}

int getItemPriority(char item)
{
	//item is lowercase: priority is 1 - 26
	//ASCII defines lowercase characters with higher int values than uppercase char's
	if (item >= 'a')
	{
		return item - 'a' + 1;
	}
	
	//item is uppercase: priority is 27 - 52
	return item - 'A' + 27;
}

int getTotalPriorityOfRucksackList(vector<string> rucksackList)
{
	int sumPriority = 0;
	char item;
	
	for (string rucksack : rucksackList)
	{
		item = identifyItemTypeAppearingInBothCompartments(rucksack);
		if (item != 0)
		{
			sumPriority += getItemPriority(item);

		}
	}

	return sumPriority;
}

char identifyItemCommonToRucksacksInGroup(vector<string> rucksackGroup)
{

	for (char item : rucksackGroup.at(0))
	{
		//exactly one item is guaranteed to be in all 3 rucksacks 
		if ((rucksackGroup.at(1).find(item) != string::npos) && (rucksackGroup.at(2).find(item) != string::npos))
		{
			return item;
		}
	}
	//if no item matches return null char (should never happen)
	return 0;
}

int getTotalPriorityOfAllGroups(vector<string> rucksackList)
{
	int sumPriority = 0;
	char item;
	vector<string> rucksackGroup(3," ");
	for (int i = 0; i < rucksackList.size(); i += 3)
	{
		for (int j = 0; j < 3; j++)
		{
			rucksackGroup.at(j) = rucksackList.at((i+j));
		}
		
		item = identifyItemCommonToRucksacksInGroup(rucksackGroup);
		if(item != 0) sumPriority += getItemPriority(item);
	}

	return sumPriority;
}

int main()
{
	vector<string> testRucksackList = preparePuzzleInput("puzzleTest.txt");
	vector<string> rucksackList	    = preparePuzzleInput("puzzleInput.txt");

	cout << "Part 1:\n";
	cout << "The sum of all rucksack priorities in test list is " << getTotalPriorityOfRucksackList(testRucksackList) << ".\n";
	cout << "The sum of all rucksack priorities in puzzle list is " << getTotalPriorityOfRucksackList(rucksackList) << ".\n";

	cout << "\n";
	cout << "Part 2:\n";
	cout << "The sum of all group priorities in test list is " << getTotalPriorityOfAllGroups(testRucksackList) << ".\n";
	cout << "The sum of all group priorities in puzzle list is " << getTotalPriorityOfAllGroups(rucksackList) << ".\n";

	return 0;
}
