// day14.cpp : Defines the entry point for the application.
//

#include "day14.h"

using namespace std;
using point = tuple<int, int>;


set<point> preparePuzzleInput(string filename)
{
	set<point> rockStructs;

	ifstream myFile(filename);

	if(myFile.is_open())
	{
		string line;
		regex arrow(" -> ");
		while (getline(myFile, line))
		{
			sregex_token_iterator it(line.begin(), line.end(), arrow, -1);

			point start,end;
			int x, y;
			size_t commaPos;
			
			//initialize first start point
			commaPos = it->str().find(',');
			x = stoi(it->str().substr(0, commaPos));
			y = stoi(it->str().substr(commaPos + 1));
			start = tuple(x, y);
			it++;
			
			for (; it != sregex_token_iterator(); it++)
			{
				commaPos = it->str().find(',');
				x = stoi(it->str().substr(0, commaPos));
				y = stoi(it->str().substr(commaPos+1));
				end = tuple(x, y);

				//vertical line
				if (get<0>(start) == get<0>(end))
				{
					int a = get<1>(start), b = get<1>(end);
					if (a > b)
					{
						a = get<1>(end), b = get<1>(start);
					}

					for (int i = a; i <= b; i++)
					{
						rockStructs.insert({ get<0>(start), i });
					}
				}
				// horizontal line
				else 
				{
					int a = get<0>(start), b = get<0>(end);
					if (a > b)
					{
						a = get<0>(end), b = get<0>(start);
					}

					for (int i = a; i <= b; i++)
					{
						rockStructs.insert({ i,get<1>(start) });
					}
				}
				start = end;
			}
		}
	}
	else
	{
		cout << "Could not open file " << filename << "\n";
		exit(1);
	}

	return rockStructs;
}

point placeOneSandUnit(const set<point>& rockFormations, const set<point>& sandRestPositions, int ymax)
{
	bool sandAtRest = false;
	int xsand = 500, ysand = 0;

	while (!sandAtRest)
	{
		//check if we can move down
		if (!(sandRestPositions.contains({ xsand,ysand + 1 }) ||
			rockFormations.contains({ xsand,ysand + 1 })))
		{
			ysand += 1;
		}
		//check if we can move down left
		else if (!(sandRestPositions.contains({ xsand - 1,ysand + 1 }) ||
			rockFormations.contains({ xsand - 1,ysand + 1 })))
		{
			xsand -= 1;
			ysand += 1;
		}
		else if (!(sandRestPositions.contains({ xsand + 1,ysand + 1 }) ||
			rockFormations.contains({ xsand + 1,ysand + 1 })))
		{
			xsand += 1;
			ysand += 1;
		}
		//we cannot move,sand is at rest
		else
		{
			sandAtRest = true;
			//cout << "sand at " << xsand << "," << ysand << "\n";
		}

		//Part1: sand fell into abyss
		//Part2: sand hit bottom and is at rest
		if ( ysand > ymax)
		{
			sandAtRest = true; //break loop
		}
		
	}
	return { xsand,ysand };
}

int pourSand(const set<point>& rockFormations)
{
	//find largest y value == Abyss boundary
	int ymax = 0;
	for (point p : rockFormations)
	{
		if (get<1>(p) > ymax) ymax = get<1>(p);
	}

	bool sandInAbyss = false;
	set<point> sandRestPositions;

	while (!sandInAbyss) 
	{
		point restPos = placeOneSandUnit(rockFormations, sandRestPositions, ymax);
		
		if (get<1>(restPos) > ymax)
		{
			sandInAbyss = true;
		}
		else
		{
			sandRestPositions.insert(restPos);
		}
		
	}

	return sandRestPositions.size();
}

int fillCaveWithSand(const set<point>& rockFormations)
{
	//find largest y value == Abyss boundary
	int ymax = 0;
	for (point p : rockFormations)
	{
		if (get<1>(p) > ymax) ymax = get<1>(p);
	}

	set<point> sandRestPositions;
	point restPos = {0,0}; //can be anything but startPos
	point startPos = { 500,0 };

	while (restPos != startPos)
	{
		restPos = placeOneSandUnit(rockFormations, sandRestPositions, ymax);
		sandRestPositions.insert(restPos);
	}

	return sandRestPositions.size();
}



int main()
{
	set<point> testRockFormations = preparePuzzleInput("puzzleTest.txt");
	set<point> puzzleRockFormations = preparePuzzleInput("puzzleInput.txt");

	cout << "Part1: \n";
	cout << "Number of units of sand at rest (test): " << pourSand(testRockFormations) << "\n";
	cout << "Number of units of sand at rest (puzzle): " << pourSand(puzzleRockFormations) << "\n";
	
	cout << "\nPart2: \n";
	cout << "Number of units of sand until flow stops (test): " << fillCaveWithSand(testRockFormations) << "\n";
	cout << "Number of units of sand until flow stops (puzzle): " << fillCaveWithSand(puzzleRockFormations) << "\n";

	return 0;
}
