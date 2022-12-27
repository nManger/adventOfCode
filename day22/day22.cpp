// day22.cpp : Defines the entry point for the application.
//

#include "day22.h"


int modulo(int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}

std::tuple<std::vector<std::string>, std::vector<std::variant<int,char>>> readPuzzleInput(std::string filename)
{
	std::vector<std::string> map;
	std::vector<std::variant<int, char>> directions;
	int deltaX = 0; 

	std::string line;
	std::ifstream myFile(filename);

	if (myFile.is_open())
	{
		//read map part of file
		getline(myFile, line);
		while (!line.empty() && !myFile.eof())
		{
			map.push_back(line);
			//keep track of largest line size for padding later
			if (line.size() > deltaX) deltaX = line.size();

			getline(myFile, line);
		}

		getline(myFile, line);

		std::regex pattern = std::regex(R"([RL])");

		auto matchIterator = std::sregex_token_iterator(line.begin(), line.end(), pattern, 0);
		auto nonMatchIterator = std::sregex_token_iterator(line.begin(), line.end(), pattern, -1);

		std::variant<int, char> placeholder;
		while (matchIterator != std::sregex_token_iterator())
		{
			placeholder = stoi(nonMatchIterator->str());
			directions.push_back(placeholder);
			placeholder = matchIterator->str()[0];
			directions.push_back(placeholder);
			++nonMatchIterator;
			++matchIterator;
		}
		directions.push_back(stoi(nonMatchIterator->str()));

		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}
	
	//pad rows to max map witdh
	for (std::string& row : map)
	{
		row.append(std::string(deltaX - row.size(), ' '));
	}

	return { map,directions };
}

std::vector<std::string> getCubeFace(const std::vector<std::string> &map, int x, int y, int delta)
{
	std::vector<std::string> face;

	for (int i = y; i < y + delta; ++i)
	{
		face.push_back(map[i].substr(x, delta));
	}

	return face;
}

//Create cube for puzzle-specific shapes: shape 0 is test, shape 1 is puzzle cube
Cube createCubeFromMap(std::vector<std::string> map, int cubeShape)
{
	Cube puzzleCube;
	puzzleCube.map = map;

	//get tile size
	int delta = std::gcd(map.size(), map.at(0).size());
	int x, y;
	Side face;


	if (cubeShape == 0)
	{
		//Top
		face = Top;
		x = 2 * delta; y = 0;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = {x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,left), std::make_tuple(Front,down), std::make_tuple(Left,down), std::make_tuple(Back,down) };

		//Back
		face = Back;
		x = 0; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Left,right), std::make_tuple(Bottom,up), std::make_tuple(Right,up),  std::make_tuple(Top,down) };

		//Left
		face = Left;
		x = delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Front,right), std::make_tuple(Bottom,right), std::make_tuple(Back,left), std::make_tuple(Top,right) };

		//Front
		face = Front;
		x = 2 * delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,down), std::make_tuple(Bottom,down), std::make_tuple(Left,left), std::make_tuple(Top,up) };

		//Bottom
		face = Bottom;
		x = 2 * delta ; y = 2*delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,right), std::make_tuple(Back,up), std::make_tuple(Left,up), std::make_tuple(Front,up) };

		//Right
		face = Right;
		x = 3 * delta ; y = 2 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.localToGlobalRotationOffset.at(face) = 1;
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Top,left), std::make_tuple(Back,right), std::make_tuple(Bottom,left), std::make_tuple(Front,left) };

	}
	else if (cubeShape == 1)
	{
		//Top
		face = Top;
		x = delta; y = 0;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,right), std::make_tuple(Front,down), std::make_tuple(Left,right), std::make_tuple(Back,right) };

		//Right
		face = Right;
		x = 2 * delta; y = 0;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Bottom,left), std::make_tuple(Front,left), std::make_tuple(Top,left), std::make_tuple(Back,up) };

		//Front
		face = Front;
		x = delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,up), std::make_tuple(Bottom,down), std::make_tuple(Left,down), std::make_tuple(Top,up) };

		//Bottom
		face = Bottom;
		x = delta; y = 2* delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Right,left), std::make_tuple(Back,left), std::make_tuple(Left,left), std::make_tuple(Front,up) };

		//Left
		face = Left;
		x = 0; y = 2 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Bottom,right), std::make_tuple(Back,down), std::make_tuple(Top,right), std::make_tuple(Front,right) };

		//Back
		face = Back;
		x = 0 ; y = 3 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = { std::make_tuple(Bottom,up), std::make_tuple(Right,down), std::make_tuple(Top,down), std::make_tuple(Left,up) };

	}

	return puzzleCube;
}

// Create cube independent from specific cube net shape
Cube createCubeFromMap(std::vector<std::string> map)
{
	//pre-compiled vector of all cube side connections (local coordinates)
	static const std::vector<std::vector<Side>> cubeSideEdges =
	{
		{Right,Front,Left,Back}, {Right,Back,Left,Front}, //Top, Bottom
		{Right,Bottom,Left,Top}, {Left,Bottom,Right,Top}, //Front, Back
		{Front,Bottom,Back,Top}, {Back,Bottom,Front,Top}   //Left, Right
	};

	//pre-compiled vector of all shifts of local vs. globa coordinate systems at cube edges (local coordinates)
	static const std::vector<std::vector<int>> cubeSideTurnCost = {{-1,0,1,2}, {1,2,-1,0}, {0,0,0,0},{0,2,0,0},{0,1,0,0},{0,-1,0,0}};

	Cube puzzleCube;
	size_t x0 = 0, y0 = 0; //top-left corner coords
	std::set<int> checkedSides;

	//get side tile size
	int delta = std::gcd(map.size(), map.at(0).size());
	
	std::queue<std::tuple<Side,int,int,int>> sidesToCheck; //new side, turn cost, x, y
	
	//1st side = top side
	x0 = map.at(0).find_first_not_of(' ');
	sidesToCheck.push({ Top,0,x0,y0 });

	while (!sidesToCheck.empty())
	{
		auto [side, cost, x, y] = sidesToCheck.front();
		sidesToCheck.pop();

		//process current side
		puzzleCube.cubeFaceCoordinateOffset.at(side) = { x,y };
		puzzleCube.cubeFaces.at(side) = getCubeFace(map, x, y, delta);

		if (cost > 2) cost -= 4; //keep cost in [-1,2]
		puzzleCube.localToGlobalRotationOffset.at(side) = cost;

		checkedSides.insert(side);

		//check if there is a tile on the right we have not checked yet
		if (x + delta != map.at(0).size() && map.at(y).at(x + delta) != ' ' 
			&& !checkedSides.contains(cubeSideEdges.at(side).at(modulo(right-cost,CARDINAL_SIZE))) )
		{
			sidesToCheck.push({ (Side) cubeSideEdges.at(side).at((modulo(right - cost, CARDINAL_SIZE))),
				cost + cubeSideTurnCost.at(side).at((modulo(right - cost, CARDINAL_SIZE))),
				x + delta, y });
		}

		//check if there is a tile to the left
		if (x - delta >= 0 && map.at(y).at(x - delta) != ' '
			&& !checkedSides.contains(cubeSideEdges.at(side).at(modulo(left - cost, CARDINAL_SIZE))))
		{
			sidesToCheck.push({ (Side) cubeSideEdges.at(side).at((modulo(left - cost, CARDINAL_SIZE))),
				cost + cubeSideTurnCost.at(side).at((modulo(left - cost, CARDINAL_SIZE))),
				x - delta, y });
		}
		//check if there is a tile below
		if (y + delta != map.size() && map.at(y+delta).at(x) != ' '
			&& !checkedSides.contains(cubeSideEdges.at(side).at(modulo(down - cost, CARDINAL_SIZE))))
		{
			sidesToCheck.push({(Side) cubeSideEdges.at(side).at((modulo(down - cost, CARDINAL_SIZE))),
				cost + cubeSideTurnCost.at(side).at((modulo(down - cost, CARDINAL_SIZE))),
				x, y + delta });
		}
		//we do not need to check for tiles above as all tiles in a row have to be connected and therefore all tiles above will have been already visited
	}

	//for all sides, get neighbours and directions in global map coordinates
	Cardinal globalHeading, localHeading, nextHeading;
	Side side, nextSide;

	for (int i = 0; i < 6; ++i)
	{
		side = (Side)i;
		
		for (int j = 0; j < CARDINAL_SIZE; ++j)
		{
			globalHeading = (Cardinal)j;
			localHeading = (Cardinal) modulo(j - puzzleCube.localToGlobalRotationOffset.at(side), CARDINAL_SIZE);

			//get next side to walk on
			nextSide = cubeSideEdges.at(side).at(localHeading);

			//find next heading
			//1. find heading we would walk on new side to get to old side
			int k;
			for (k = 0; k < CARDINAL_SIZE; ++k)
			{
				if (cubeSideEdges.at(nextSide).at(k) == side) break;
			}
			//2. flip heading and rotate back to global coordinate system
			nextHeading = (Cardinal) modulo(k + 2 + puzzleCube.localToGlobalRotationOffset.at(nextSide), CARDINAL_SIZE);

			puzzleCube.edgeDirectionChanges.at(side).at(globalHeading) = { nextSide,nextHeading };

		}

	}

	return puzzleCube;
}


long long walkCube(const Cube& mapCube, const std::vector<std::variant<int, char>>& directions)
{
	auto [xglobal, yglobal] = mapCube.cubeFaceCoordinateOffset[Top];
	int delta = mapCube.cubeFaces[Top].size();
	int x = 0, y = 0;
	Cardinal heading = right, nextHeading = heading;
	Side currentSide = Top , nextSide = currentSide;
	bool changeSide = false;
	
	for (auto& direction : directions)
	{
		//std::cout << x << " " << y << " " << heading << "\n";
		//change heading
		if (std::holds_alternative<char>(direction))
		{
			if (std::get<char>(direction) == 'R')
			{
				heading = (Cardinal)modulo(heading + 1, CARDINAL_SIZE);
			}
			else
			{
				heading = (Cardinal)modulo(heading - 1, CARDINAL_SIZE);
			}
			nextHeading = heading;
		}
		else
		{
			int dist = std::get<int>(direction);
			int tmpX, tmpY;
			

			for (int i = 0; i < dist; ++i)
			{
				changeSide = false;

				if (heading == right)
				{
					//get next possible position
					if (x + 1 == delta) //we reached end of current face 
					{
						changeSide = true;
						std::tie(nextSide,nextHeading) = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
						if (nextHeading == right) { tmpX = 0; tmpY = y; }
						else if (nextHeading == left) { tmpX = delta - 1, tmpY = delta - 1 - y; }
						else if (nextHeading == up) { tmpX = y; tmpY = delta - 1; }
						else { tmpX = delta - 1 - y; tmpY = 0; } //down
					}
					else //continue on current face
					{ 
						tmpX = x + 1; tmpY = y; 
					}
					
					//check if next position is allowed, stop if not
					if (mapCube.cubeFaces.at(nextSide)[tmpY][tmpX] == '#'){
						//reset next side as we stay on current side after all
						nextSide = currentSide;
						break;
						}
					else 
					{ 
						x = tmpX; y = tmpY;
						if (changeSide)
						{
							currentSide = nextSide;
							heading = nextHeading;
						}
					}
					
				}
				else if(heading == left)
				{
					//get next position
					//if we reached side of current face, get next one
					if (x - 1 < 0) 
					{ 
						changeSide = true;
						std::tie(nextSide, nextHeading) = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
						if (nextHeading == right) { tmpX = 0; tmpY = delta - 1 - y; }
						else if (nextHeading == left) { tmpX = delta - 1, tmpY = y; }
						else if (nextHeading == up) { tmpX = delta - 1 - y; tmpY = delta - 1; }
						else { tmpX = y; tmpY = 0; } //down
					}
					//continue on current face
					else 
					{
						tmpX = x - 1;
						tmpY = y;
					}	

					//check if next position is allowed, else stop walking
					if (mapCube.cubeFaces.at(nextSide)[tmpY][tmpX] == '#')
					{
						nextSide = currentSide;
						break;
					}
					else
					{
						x = tmpX;
						y = tmpY;
						if (changeSide)
						{
							heading = nextHeading;
							currentSide = nextSide;
						}
					}
				}
				else if (heading == up)
				{
					//get next position
					//if we reached side of current face, get next one
					if (y - 1 < 0)
					{
						changeSide = true;
						std::tie(nextSide, nextHeading) = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
						if (nextHeading == right) { tmpX = 0; tmpY = x; }
						else if (nextHeading == left) { tmpX = delta - 1; tmpY = delta - 1 -x; }
						else if (nextHeading == up) { tmpX = x; tmpY = delta - 1; }
						else { tmpX = delta - 1 - x; tmpY = 0; } //down
					}
					//continue on current face
					else
					{
						tmpX = x;
						tmpY = y - 1;
					}

					//check if next position is allowed, else stop walking
					if (mapCube.cubeFaces.at(nextSide)[tmpY][tmpX] == '#')
					{
						nextSide = currentSide;
						break;
					}
					else
					{
						x = tmpX;
						y = tmpY;
						if (changeSide)
						{
							heading = nextHeading;
							currentSide = nextSide;
						}
					}
					
				}
				else if (heading == down)
				{
					//get next position
					//if we reached side of current face, get next one
					if (y + 1 == delta)
					{
						changeSide = true;
						std::tie(nextSide, nextHeading) = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
						if (nextHeading == right) { tmpX = 0; tmpY = delta - 1 - x; }
						else if (nextHeading == left) { tmpX = delta - 1; tmpY = x; }
						else if (nextHeading == up) { tmpX = delta - 1 - x ; tmpY = delta - 1; }
						else { tmpX = x; tmpY = 0; } //down
					}
					//continue on current face
					else
					{
						tmpX = x;
						tmpY = y + 1;
					}

					//check if next position is allowed, else stop walking
					if (mapCube.cubeFaces.at(nextSide)[tmpY][tmpX] == '#')
					{
						nextSide = currentSide;
						break;
					}
					else
					{
						x = tmpX;
						y = tmpY;
						if (changeSide)
						{
							heading = nextHeading;
							currentSide = nextSide;
						}
					}
				}
			}
		}
	}

	xglobal = get<0>(mapCube.cubeFaceCoordinateOffset.at(currentSide)) + x;
	yglobal = get<1>(mapCube.cubeFaceCoordinateOffset.at(currentSide)) + y;

	std::cout <<"(" << xglobal + 1 << " " << yglobal + 1 << " " << heading << ") ";
	return (1000ll * (yglobal + 1) + 4 * (xglobal + 1) + heading);
}

long long walkMap(const std::vector<std::string>& map, const std::vector<std::variant<int, char>>& directions)
{
	int x = 0, //x increases right
		y = 0, //y increases down
		deltaX = map.at(0).size(),
		deltaY = map.size();

	Cardinal heading = right;

	//find initial x position
	x = map.at(0).find_first_not_of(' ');

	for (auto& direction : directions)
	{
		//std::cout << x << " " << y << " " << heading << "\n";
		//change heading
		if (std::holds_alternative<char>(direction))
		{
			if (std::get<char>(direction) == 'R')
			{
				heading = (Cardinal) modulo(heading + 1, CARDINAL_SIZE);
			}
			else
			{
				heading = (Cardinal) modulo(heading - 1, CARDINAL_SIZE);
			}
		}
		else
		{
			int dist = std::get<int>(direction);
			int tmp;
			bool stopped = false;

			for (int i = 0; i < dist; ++i)
			{
				if (stopped) break;

				switch (heading)
				{
				case(right):
					if (x + 1 == deltaX || map[y][x + 1] == ' ') tmp = map.at(y).find_first_of(".#"); else tmp = x + 1;
					if (map[y][tmp] == '#') stopped = true; else x = tmp;
					break;
				case(left):
					if (x - 1 < 0 || map[y][x - 1] == ' ') tmp = map.at(y).find_last_of(".#"); else tmp = x - 1;
					if (map[y][tmp] == '#') stopped = true; else x = tmp;
					break;
				case(up):
					if (y - 1 < 0 || map[y - 1][x] == ' ')
					{
						tmp = deltaY - 1;
						while ( map[tmp][x] == ' ')
						{
							--tmp;
						}
					}
					else
					{
						tmp = y - 1;
					}
					if (map[tmp][x] == '#') stopped = true; else y = tmp;
					break;
				case(down):
					if (y + 1 == deltaY ||  map[y + 1][x] == ' ')
					{
						tmp = 0;
						while (map[tmp][x] == ' ')
						{
							++tmp;
						}
					}
					else
					{
						tmp = y + 1;
					}
					if (map[tmp][x] == '#') stopped = true; else y = tmp;
					break;
				}
			}
		}
	}
	
	std::cout <<"("<< x + 1 << " " << y + 1 << " " << heading << ") ";
	return (1000ll*(y+1) + 4*(x+1) + heading);
}

int main()
{
	auto [testMap, testDirections] = readPuzzleInput("puzzleTest.txt");
	auto [map, directions] = readPuzzleInput("puzzleInput.txt");
	
	std::cout << "Part 1:\n";
	std::cout << "Final password (test) is: " << walkMap(testMap, testDirections) << "\n";
	std::cout << "Final password (puzzle) is: " << walkMap(map, directions) << "\n";

	Cube testCube = createCubeFromMap(testMap);
	Cube puzzleCube = createCubeFromMap(map);

	std::cout << "Part 2:\n";
	std::cout << "Final password (test) is: " << walkCube(testCube, testDirections) << "\n";
	std::cout << "Final password (puzzle) is: " << walkCube(puzzleCube, directions) << "\n";
	
	//write out cube structure
	/*
	Cube testCube = createCubeFromMap(map);
	for (int i = 0; i < 6; ++i)
	{
		std::cout << "Side: " << i << " Offset: " << testCube.localToGlobalRotationOffset[i];
		std::cout << " x: "<< get<0>(testCube.cubeFaceCoordinateOffset.at(i))<< " y: " << get<1>(testCube.cubeFaceCoordinateOffset.at(i)) << "\n";
		std::cout << "Next Side/Heading : ";
		for (auto [side,heading]: testCube.edgeDirectionChanges.at(i))
		{
			std::cout << side << "/" << heading << " ";
		}
		std::cout << "\n";
	}*/

	return 0;
}
