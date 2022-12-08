// day8.cpp : Defines the entry point for the application.
//

#include "day8.h"

using namespace std;

vector<vector<short>> readTreeMapFromFile(const string& filename)
{
	vector<vector<short>> treeMap;

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			vector<short> row;
			for (char c : line)
			{
				row.push_back(c - '0');
			}
			treeMap.push_back(row);
		}
		myfile.close();
	}
	else
	{
		cout << "Could not open " << filename << ".\n";
	}

	return treeMap;
}

/*
* Determine if tree at pos is visible from outside the grid.
* Tree is visible if all trees in one direction are smaller than it.
*/
bool treeIsVisble(const vector<vector<short>>& treeMap,tuple<int,int> treePos)
{
	auto [ i, j] = treePos;
	bool isVisibleInDirection = true;
	
	//search top
	for (int k = i-1; k >= 0; k--)
	{
		//found higer tree, terminate search
		if (treeMap.at(k).at(j) >= treeMap.at(i).at(j))
		{
			isVisibleInDirection = false;
			break;
		}
	}
	if (isVisibleInDirection) return true;
	
	//search bottom
	isVisibleInDirection = true;
	for (int k = i+1; k < treeMap.size(); k++)
	{
		//found higer tree, terminate search
		if (treeMap.at(k).at(j) >= treeMap.at(i).at(j))
		{
			isVisibleInDirection = false;
			break;
		}
	}
	if (isVisibleInDirection) return true;

	//search left
	isVisibleInDirection = true;
	for (int l = j-1; l >= 0; l--)
	{
		//found higer tree, terminate search
		if (treeMap.at(i).at(l) >= treeMap.at(i).at(j))
		{
			isVisibleInDirection = false;
			break;
		}
	}
	if (isVisibleInDirection) return true;

	//search right
	isVisibleInDirection = true;
	for (int l = j+1; l < treeMap.at(0).size(); l++)
	{
		//found higer tree, terminate search
		if (treeMap.at(i).at(l) >= treeMap.at(i).at(j))
		{
			isVisibleInDirection = false;
			break;
		}
	}
	if (isVisibleInDirection) return true;

	return false;
}

/* 
* Count the number of trees visible from outside the treeMap grid.
* Assumes tree grid is rectangular.
*/
int countVisibleTrees(const vector<vector<short>>& treeMap)
{
	//initialize number of visible trees to number of edge trees, as edge trees are visible by default
	int visibleTrees = 2 * (treeMap.size() + treeMap.at(0).size() - 2);

	//iterate over inner trees
	for (int i = 1; i < treeMap.size() - 1; i++)
	{
		for (int j = 1; j < treeMap.at(0).size() - 1; j++)
		{
			if (treeIsVisble(treeMap, make_tuple(i, j)))
			{
				visibleTrees += 1;
			}
		}
	}
	return visibleTrees;
}

/* 
* Calculate scenic score of tree at given position
* as the product of number of trees visible in each direction.
*/
int getTreeScenicScore(const vector<vector<short>>& treeMap, tuple<int, int> treePos)
{
	auto [i, j] = treePos;
	int viewingDistance = 0;
	int scenicScore = 1;

	//search top
	for (int k = i - 1; k >= 0; k--)
	{
		viewingDistance++;
		//found higer tree, terminate search
		if (treeMap.at(k).at(j) >= treeMap.at(i).at(j))
		{
			break;
		}
	}
	scenicScore *= viewingDistance;

	//search bottom
	viewingDistance = 0;
	for (int k = i + 1; k < treeMap.size(); k++)
	{
		viewingDistance++;
		//found higer tree, terminate search
		if (treeMap.at(k).at(j) >= treeMap.at(i).at(j))
		{
			break;
		}
	}
	scenicScore *= viewingDistance;

	//search left
	viewingDistance = 0;
	for (int l = j - 1; l >= 0; l--)
	{
		viewingDistance++;
		//found higer tree, terminate search
		if (treeMap.at(i).at(l) >= treeMap.at(i).at(j))
		{
			break;
		}
	}
	scenicScore *= viewingDistance;

	//search right
	viewingDistance = 0;
	for (int l = j + 1; l < treeMap.at(0).size(); l++)
	{
		viewingDistance++;
		//found higer tree, terminate search
		if (treeMap.at(i).at(l) >= treeMap.at(i).at(j))
		{
			break;
		}
	}
	scenicScore *= viewingDistance;

	return scenicScore;
}

int findHighestScenicScore(const vector<vector<short>>& treeMap)
{
	int maxScenicScore=0;
	int currentScenicScore;

	//trees at edge have score 0, ignore them
	for (int i = 1; i < treeMap.size() - 1; i++)
	{
		for (int j = 1; j < treeMap.at(0).size() - 1; j++)
		{
			currentScenicScore = getTreeScenicScore(treeMap, make_tuple(i, j));
			if (currentScenicScore > maxScenicScore)
			{
				maxScenicScore = currentScenicScore;
			}
		}
	}
	return maxScenicScore;
}

int main()
{
	vector<vector<short>> testTreeMap = readTreeMapFromFile("puzzleTest.txt");
	vector<vector<short>> puzzleTreeMap = readTreeMapFromFile("puzzleInput.txt");

	cout << "Part 1:\n";
	cout << "Number of visible trees in test tree map is " << countVisibleTrees(testTreeMap) << ".\n";
	cout << "Number of visible trees in puzzle tree map is " << countVisibleTrees(puzzleTreeMap) << ".\n";

	cout << "\n Part 2:\n";
	cout << "Highest scenic score in test tree map is " << findHighestScenicScore(testTreeMap) << ".\n";
	cout << "Highest scenic score in puzzle tree map is " << findHighestScenicScore(puzzleTreeMap) << ".\n";

	return 0;
}
