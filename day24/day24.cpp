// day24.cpp : Defines the entry point for the application.
//

#include "day24.h"
using grid = std::vector<std::vector<std::string>>;

void drawGrid(const grid &grove)
{
	std::cout << "\n";
	for (auto& row : grove)
	{
		for (std::string s : row)
		{
			if (s.size() > 1) std::cout << s.size(); //print number of blizzards should more than one overlap
			else std::cout << s;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

grid readGroveGrid(std::string filename)
{
	grid grove;

	std::ifstream myFile(filename);
	std::string line;
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			grove.push_back(std::vector<std::string>());
			
			for (char c : line)
			{
				grove.back().push_back(std::string(1, c));
			}
		}
		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}

	return grove;
}

grid advanceBlizzards(const grid &grove)
{
	//create new empty grove
	grid advancedGrove = std::vector(grove.size(), std::vector(grove[0].size(), std::string()));

	int deltaX = grove[0].size(),
		deltaY = grove.size();

	for (int i = 0; i < deltaY; ++i)
	{
		for (int j = 0; j < deltaX; ++j)
		{
			//copy boundaries]
			if (grove[i][j][0] == '#')
			{
				advancedGrove[i][j] = std::string(1,'#');
			}
			//copy empty floor if grid point is not yet occupied by blizzard
			else if (grove[i][j][0] == '.')
			{
				if(advancedGrove[i][j].empty())
				{
					advancedGrove[i][j] = std::string(1, '.');
				}
			}
			//copy and move blizzards
			else
			{
				// when moving blizzards, only points <i and =i,<j are guaranteed to be occupied and can have empyt-space tiles we need to overwrite
				// also, we assumme that tile i,j is now empty space if no blizzard has already moved there
				if (advancedGrove[i][j].empty())
				{
					advancedGrove[i][j] = std::string(1, '.');
				}

				for (char c : grove[i][j])
				{
					//blizzard moves right
					if (c == '>')
					{
						//if grid point to right is still accessible move there
						if (j + 1 < deltaX - 1)
						{
							advancedGrove[i][j + 1].push_back(c);
						}
						//else wrap around to leftmost accessible grid point in row
						else
						{
							//check if grid point contains empty ground, overwrite if true, else append
							if (advancedGrove[i][1].size() > 0 && advancedGrove[i][1][0] == '.')
							{
								advancedGrove[i][1][0] = c;
							}
							else
							{
								advancedGrove[i][1].push_back(c);
							}
						}
					}
					//blizzard moves left
					else if (c == '<')
					{
						//if grid point to left is accessible, move there
						if (j - 1 > 0)
						{
							//if new point is empty floor, overwrite, else append
							if (advancedGrove[i][j-1][0] == '.')
							{
								advancedGrove[i][j-1][0] = c;
							}
							else
							{
								advancedGrove[i][j-1].push_back(c);
							}
						}
						//else wrap around to rightmost accessible point in grid
						else
						{
							advancedGrove[i][deltaX - 2].push_back(c);
						}
					}
					//blizzard moves up
					else if (c == '^')
					{
						//if grid point above is accessible, move there
						if (i - 1 > 0)
						{
							//if new point is empty floor, overwrite, else append
							if (advancedGrove[i-1][j][0] == '.')
							{
								advancedGrove[i-1][j][0] = c;
							}
							else
							{
								advancedGrove[i-1][j].push_back(c);
							}
						}
						//else wrap around to first accessible tile from bottom up
						else
						{
							advancedGrove[deltaY - 2][j].push_back(c);
						}
					}
					//blizzard moves down
					else //if(c == 'v')
					{
						//if grid point below is accessible, move there
						if (i + 1 < deltaY - 1)
						{
							advancedGrove[i + 1][j].push_back(c);
						}
						//else wrap around to first accessible tile from top down
						else
						{
							//if new point is empty floor, overwrite, else append
							if (advancedGrove[1][j][0] == '.')
							{
								advancedGrove[1][j][0] = c;
							}
							else
							{
								advancedGrove[1][j].push_back(c);
							}
						}

					}

				}
			}

		}
	}

	return advancedGrove;
}


int findShortestPathThoughGrove(const grid &initialGrove, int totalWaypoints)
{
	grid groveGridInTime = initialGrove;
	std::queue<std::tuple<int, int, int>> searchQueue; //queue points to search as {x,y,time}

	//find start and destination points
	int xInit = 0, // x increases right
		yInit = 0, // y increases down
		xDest = 0,
		yDest = initialGrove.size()-1;

	for (int n = 0; n < initialGrove[0].size(); ++n)
	{
		if (initialGrove[0][n][0] == '.') xInit = n;
		if (initialGrove[yDest][n][0] == '.') xDest = n;
	}

	//walk grove
	int timeMinutes = 0;
	int waypointsReached = 0; //track number of waypoints reached
	searchQueue.push({ xInit,yInit,1 });
	std::set<std::tuple<int, int>> possiblePositions; //keep track of positions so we don't check a position multiple times for one timestep

	//terminate loop if we run out of possible moves or if the 3rd waypoint is reached
	while (!searchQueue.empty() && waypointsReached < totalWaypoints)
	{
		//get contents of first element in queue and remove it
		auto [x, y, t] = searchQueue.front();
		searchQueue.pop();
		
		//if new node is advanced in time, update grove map, reset position tracker
		if (t > timeMinutes)
		{
			++timeMinutes;
			groveGridInTime = advanceBlizzards(groveGridInTime);
			//std::cout << timeMinutes << "\n";
			possiblePositions.clear(); 
			//drawGrid(groveGridInTime);
		}

		//check if we move into destination point
		//if so, increase waypoint counter, flip init and dest positions
		//and reset queue (as we are interrested in the sum of the shortest paths between each waypoint pair),
		//then skip ahead to next loop cycle
		if (x == xDest && ( y + 1 == yDest || y - 1 == yDest ))
		{
			++waypointsReached;
			//std::cout << "Reached waypoint " << waypointsReached << ", time " << timeMinutes << "\n";

			int dummy = yDest; yDest = yInit; yInit = dummy;
			dummy = xDest; xDest = xInit; xInit = dummy;

			searchQueue = std::queue<std::tuple<int, int, int>>();
			searchQueue.push({ xInit,yInit,t + 1 });

			continue;
		}

		//check if we can move down
		if (y < initialGrove.size() - 1 && groveGridInTime[y + 1][x][0] == '.' && !possiblePositions.contains({x,y+1}))
		{
			searchQueue.push({ x,y+1,t + 1 });
			possiblePositions.insert({ x,y + 1 });
		}

		//check if we can move up
		if (y != 0 && groveGridInTime[y - 1][x][0] == '.' && !possiblePositions.contains({ x,y - 1 }))
		{
			searchQueue.push({ x,y - 1,t + 1 });
			possiblePositions.insert({ x,y - 1 });
		}
		//check if we can move right
		if (groveGridInTime[y][x+1][0] == '.' && !possiblePositions.contains({ x + 1,y }))
		{
			searchQueue.push({ x+1,y,t + 1 });
			possiblePositions.insert({ x + 1,y});
		}
		//check if we can move left
		if (groveGridInTime[y][x-1][0] == '.' && !possiblePositions.contains({ x - 1,y }))
		{
			searchQueue.push({ x-1,y ,t + 1 });
			possiblePositions.insert({ x - 1,y });
		}

		//check if we can stay (we can only stay at init if no other move is possible)
		if (groveGridInTime[y][x][0] == '.' && (y != yInit || possiblePositions.empty()) && !possiblePositions.contains({ x,y }))
		{
			searchQueue.push({ x,y,t + 1 });
			possiblePositions.insert({ x,y });
		}
	}

	if (searchQueue.empty())
	{
		std::cout << "Destination has not been reached. \n";
	}

	return timeMinutes;
}

int main()
{
	grid testGrove = readGroveGrid("puzzleTest.txt");
	grid puzzleGrove = readGroveGrid("puzzleInput.txt");

	std::cout << "Part 1:\n";
	std::cout << "Shortest path through grove (test) takes " << findShortestPathThoughGrove(testGrove,1) << " minutes.\n";
	std::cout << "Shortest path through grove (puzzle) takes " << findShortestPathThoughGrove(puzzleGrove,1) << " minutes.\n";

	std::cout << "Part 1:\n";
	std::cout << "Shortest path through grove and to retrieve snacks (test) takes " << findShortestPathThoughGrove(testGrove, 3) << " minutes.\n";
	std::cout << "Shortest path through grove and to retrieve snacks (puzzle) takes " << findShortestPathThoughGrove(puzzleGrove, 3) << " minutes.\n";

	
	/*
	drawGrid(testGrove);
	for (int i = 0; i < 5; ++i)
	{
		testGrove = advanceBlizzards(testGrove);
		drawGrid(testGrove);
	}
	*/

	return 0;
}
