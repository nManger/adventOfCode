// day23.cpp : Defines the entry point for the application.
//

#include "day23.h"

using point = std::tuple<int, int>;

enum Cardinal {north,south,west,east};

//get next heading as given by increasing order in enum Cardinal
//wraps around to first heading
Cardinal newHeading(Cardinal oldHeading)
{
	return (Cardinal) ((oldHeading + 1) % 4);
}

std::vector<point> readPuzzleInput(std::string filename)
{
	std::vector<point> elfPositions;

	//set {0,0} arbitrarily at top left of input map
	int x = 0, //x increases right/east
		y = 0; //y increases down/south

	std::ifstream myFile(filename);
	std::string line;
	
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			x = 0; //reset x for new line
			for (char c : line)
			{
				if (c == '#') elfPositions.push_back({ x,y });
				++x;
			}
			++y;
		}

		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + "\n";
		exit(1);
	}

	return elfPositions;
}

std::tuple<std::tuple<int, int>, std::tuple<int, int>> getElfGridBoundingBox(const std::set<point>& elfPositions)
{
	int xmin = std::numeric_limits<int>::max(),
		xmax = std::numeric_limits<int>::min(),
		ymin = std::numeric_limits<int>::max(),
		ymax = std::numeric_limits<int>::min();

	for (auto [x, y] : elfPositions)
	{
		//std::cout << x << " " << y << "\n";
		if (x < xmin) xmin = x;
		if (x > xmax) xmax = x;
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
	}

	return { {xmin,xmax},{ymin,ymax} };
}

void drawElfGrid(const std::set<point>& elfPositions)
{
	auto [xbound, ybound] = getElfGridBoundingBox(elfPositions);
	int deltaX = get<1>(xbound) - get<0>(xbound) + 1;
	int deltaY = get<1>(ybound) - get<0>(ybound) + 1;

	std::cout << get<1>(xbound) << " " << get<0>(xbound) << " " << get<1>(ybound) << " " << get<0>(ybound) << "\n";

	std::vector<std::vector<char>> grid = std::vector(deltaY, std::vector(deltaX, '.'));

	for (auto& [x, y] : elfPositions)
	{
		grid.at(y - get<0>(ybound)).at(x - get<0>(xbound)) = '#';
	}

	for (int i = 0; i < deltaY; ++i)
	{
		for (int j = 0; j < deltaX; ++j)
		{
			std::cout << grid[i][j];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool moveElvesOneRound(std::set<point>& elfPositions, const Cardinal initalHeading)
{
	std::map<point, point> proposedPositions; 
	std::set<point> elfNeighbours;
	Cardinal heading;
	bool elvesMoved = false;

	//Part 1: get proposed positions from all elves (non-moving elves don't propose positions)
	for (auto it = elfPositions.begin(); it!=elfPositions.end(); ++it)
	{
		auto [x, y] = *it;
		heading = initalHeading;
		elfNeighbours.clear();

		//find elf's neighbours
		auto lowerSearchBound = elfPositions.lower_bound({ x - 1,y - 1 });
		auto upperSearchBound = elfPositions.upper_bound({ x + 1,y + 1 });

		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue; //skip elf itself
				point pos = {x+i,y+j};
				for (auto jt = lowerSearchBound; jt !=upperSearchBound; ++jt)
				{
					if ((*jt) == pos) elfNeighbours.insert((*jt));
				}
			}
		}
		
		//if elf has no neighbours he does not move
		//and we do not have to consider him as neighbour for any following elf
		if (elfNeighbours.empty())
		{
			continue;
		}

		elvesMoved = true;

		//check each heading in order specified by enum Cardinal
		bool headingBlocked = false;
		point newPosition;
		for (int n = 0; n < 4; n++)
		{
			headingBlocked = false;
			if (heading == north)
			{	
				if (elfNeighbours.contains({ x - 1,y - 1 }) ||
					elfNeighbours.contains({ x,y - 1 }) ||
					elfNeighbours.contains({ x + 1,y - 1 }))
				{
					headingBlocked = true;
				}
				else //heading is not blocked, save position to propose
				{
					newPosition = { x, y - 1 };
				}
			}
			else if (heading == south)
			{
				if (elfNeighbours.contains({ x - 1,y + 1 }) ||
					elfNeighbours.contains({ x,y + 1 }) ||
					elfNeighbours.contains({ x + 1,y + 1 }))
				{
					headingBlocked = true;
				}
				else //heading is not blocked, save position to propose
				{
					newPosition = { x, y + 1 };
				}
			}
			else if (heading == west)
			{
				if (elfNeighbours.contains({ x - 1,y - 1 }) ||
					elfNeighbours.contains({ x - 1,y }) ||
					elfNeighbours.contains({ x - 1,y + 1 }))
				{
					headingBlocked = true;
				}
				else //heading is not blocked, save position to propose
				{
					newPosition = { x - 1, y };
				}
			}
			else //heading == east
			{
				if (elfNeighbours.contains({ x + 1,y - 1 }) ||
					elfNeighbours.contains({ x + 1,y }) ||
					elfNeighbours.contains({ x + 1,y + 1 }))
				{
					headingBlocked = true;
				}
				else //heading is not blocked, save position to propose
				{
					newPosition = { x + 1, y };
				}
			}

			//we found possible direction
			if (!headingBlocked) break;

			//get next heading
			heading = newHeading(heading);
		}

		if (!headingBlocked)
		{
			//if position is already here, erase it
			//this is valid as a maximum of two points can propose to move into one spot
			//(points 2 steps apart and proposing to move towards each other)
			if (auto pos = proposedPositions.find(newPosition); pos != proposedPositions.end())
			{
				proposedPositions.erase(pos);
			}
			//otherwise store new and old position;
			else
			{
				proposedPositions.insert({ newPosition, (*it) });
			}
		}
	}

	//Part 2: for all moving elves, erase old position (second map element) and insert new one (first map element)
	for (auto it = proposedPositions.begin(); it != proposedPositions.end(); ++it)
	{
		elfPositions.erase(it->second);
		elfPositions.insert(it->first);
	}
	return elvesMoved;
}

int moveElvesNRounds(const std::vector<point>& elfPositions, int rounds)
{
	std::set<point> elfNewPositions = std::set(elfPositions.begin(),elfPositions.end());
	Cardinal currentInitHeading = north;

	for (int i = 0; i < rounds; ++i)
	{
		//move elves by one
		moveElvesOneRound(elfNewPositions,currentInitHeading);
		
		//drawElfGrid(elfNewPositions);
		
		//get next heading
		currentInitHeading = newHeading(currentInitHeading);
	}

	//get size of grid occupied by elves
	auto [xbound, ybound] = getElfGridBoundingBox(elfNewPositions);
	long long deltaX = get<1>(xbound) - get<0>(xbound) + 1;
	long long deltaY = get<1>(ybound) - get<0>(ybound) + 1;

	return (deltaX * deltaY) - elfNewPositions.size();

}

int distributeElves(const std::vector<point>& elfPositions)
{
	std::set<point> elfNewPositions = std::set(elfPositions.begin(),elfPositions.end());
	Cardinal currentInitHeading = north;
	bool elvesMoved = true;
	int rounds = 0;

	while (elvesMoved)
	{
		elvesMoved = moveElvesOneRound(elfNewPositions, currentInitHeading);

		currentInitHeading = newHeading(currentInitHeading);
		++rounds;
	}
	return rounds;
}

int main()
{
	std::vector<point> testPositions = readPuzzleInput("puzzleTest.txt");
	std::vector<point> elfPositions = readPuzzleInput("puzzleInput.txt");

	std::cout << "Part 1:\n";
	std::cout << "Number of free tiles in test grid: "<< moveElvesNRounds(testPositions, 10) <<"\n";
	std::cout << "Number of free tiles in puzzle grid: " << moveElvesNRounds(elfPositions, 10) << "\n";

	std::cout << "part 1:\n";
	std::cout << "number of rounds until all elves are stationary (test): " << distributeElves(testPositions) << "\n";
	std::cout << "number of rounds until all elves are stationary (puzzle): " << distributeElves(elfPositions) << "\n";
	
	return 0;
}
