﻿// day22.cpp : Defines the entry point for the application.
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

//cube shape 0 is test, 1 is puzzle cube
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
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { left,down,down,down };

		//Back
		face = Back;
		x = 0; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { right,up,up,down };

		//Left
		face = Left;
		x = delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { right,right,left,right };

		//Front
		face = Front;
		x = 2 * delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { down,down,left,up };

		//Bottom
		face = Bottom;
		x = 2 * delta ; y = 2*delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { right,up,up,up };

		//Right
		face = Right;
		x = 3 * delta ; y = 2 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = right;
		puzzleCube.localToGlobalRotationOffset.at(face) = 1;
		puzzleCube.edgeDirectionChanges.at(face) = { left,right,left,left };

	}
	else if (cubeShape == 1)
	{
		//Top
		face = Top;
		x = delta; y = 0;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { right,down,right,right };

		//Right
		face = Right;
		x = 2 * delta; y = 0;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = left;
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = { left,left,left,up };

		//Front
		face = Front;
		x = delta; y = delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { up,down,down,up };

		//Bottom
		face = Bottom;
		x = delta; y = 2* delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = up;
		puzzleCube.edgeDirectionChanges.at(face) = { left,left,left,up };

		//Left
		face = Left;
		x = 0; y = 2 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = left;
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = { right,down,right,right };

		//Back
		face = Back;
		x = 0 ; y = 3 * delta;
		puzzleCube.cubeFaceCoordinateOffset.at(face) = { x,y };
		puzzleCube.cubeFaces.at(face) = getCubeFace(map, x, y, delta);
		puzzleCube.faceUpSideDirection.at(face) = left;
		puzzleCube.localToGlobalRotationOffset.at(face) = -1;
		puzzleCube.edgeDirectionChanges.at(face) = {up ,down,down,up };

	}

	return puzzleCube;
}

/* Create cube independent from specific net (unfinished)
Cube createCubeFromMap(std::vector<std::string> map)
{
	Cube puzzleCube;
	size_t x = 0, y = 0; //top-left corner coords
	Side side, prevSide;
	std::set<Side> checkedSides;

	//get side tile size
	int delta = std::gcd(map.size(), map.at(0).size());
	
	
	std::queue<std::tuple<Side,Side,int,int>> sidesToCheck; //new side, prev side, x, y
	//1st side = top side

	side = Top;
	x = map.at(0).find_first_not_of(' ');
	sidesToCheck.push({ side,side,x,y });

	while (!sidesToCheck.empty())
	{
		auto [side, prevSide, x, y] = sidesToCheck.front();
		sidesToCheck.pop();
		
		//process current side
		puzzleCube.cubeFaceCoordinateOffset.at(side) = { x,y };
		puzzleCube.cubeFaces.at(side) = getCubeFace(map, x, y, delta);

		//find relation of prev to current tile
		int topside;
		if (side == prevSide) topside = up; //1st side (Top side) exception
		//side is right of prev
		else if (x > get<0>(puzzleCube.cubeFaceCoordinateOffset[prevSide]))
		{
			if (prevSide == Top) 
			{
				puzzleCube.
			}
			else if(prevSide == Bottom)
			{

			}
			else
			{

			}
		}
		//side is left of prev
		else if (x < get<0>(puzzleCube.cubeFaceCoordinateOffset[prevSide]))
		{

		}
		//side is below prev
		else
		{

		}

		puzzleCube.faceUpSideDirection.at(side) = puzzleCube.faceUpSideDirection.at(prevSide);
		checkedSides.insert(side);
	}
	
	prevSide = side;

	if(x + delta + 1 > map.at(0).size() || map.at(0).at(x + delta + 1) == ' ')
	{
		y = y + delta + 1;
		side = cubeSideEdges.at(prevSide).at(down);
		puzzleCube.faceUpSideDirection.at(side) = puzzleCube.faceUpSideDirection.at(prevSide);
	}
	else
	{
		x = x + delta + 1;
		side = cubeSideEdges.at(prevSide).at(right);
		puzzleCube.faceUpSideDirection.at(side) = right;
		
	}
	puzzleCube.cubeFaces.at(side) = getCubeFace(map, x, y, delta);
	puzzleCube.cubeFaceCoordinateOffset.at(side) = { x,y };
	checkedSides.insert(side);
	prevSide = side;




	return puzzleCube;
}
*/


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
						nextSide = cubeSideEdges.at(currentSide).at(modulo(heading - mapCube.localToGlobalRotationOffset.at(currentSide), CARDINAL_SIZE));
						nextHeading = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
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
						nextSide = cubeSideEdges.at(currentSide).at(modulo(heading - mapCube.localToGlobalRotationOffset.at(currentSide), CARDINAL_SIZE));
						nextHeading = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
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
						nextSide = cubeSideEdges.at(currentSide).at(modulo(heading - mapCube.localToGlobalRotationOffset.at(currentSide), CARDINAL_SIZE));
						nextHeading = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
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
						nextSide = cubeSideEdges.at(currentSide).at(modulo(heading - mapCube.localToGlobalRotationOffset.at(currentSide), CARDINAL_SIZE));
						nextHeading = mapCube.edgeDirectionChanges.at(currentSide).at(heading);
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

	std::cout << xglobal << " " << yglobal <<" " << heading << "\n";
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
	
	//std::cout << x << " " << y <<" " << heading << "\n";
	return (1000ll*(y+1) + 4*(x+1) + heading);
}

int main()
{
	auto [testMap, testDirections] = readPuzzleInput("puzzleTest.txt");
	auto [map, directions] = readPuzzleInput("puzzleInput.txt");
	
	std::cout << "Part 1:\n";
	//std::cout << "Final password (test) is: " << walkMap(testMap, testDirections) << "\n";
	//std::cout << "Final password (puzzle) is: " << walkMap(map, directions) << "\n";

	//Cube testCube = createCubeFromMap(testMap,0);
	//std::cout << walkCube(testCube, testDirections) << '\n';
	Cube puzzleCube = createCubeFromMap(map, 1);
	std::cout << walkCube(puzzleCube, directions) << '\n';


	return 0;
}
