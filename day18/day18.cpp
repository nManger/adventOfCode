// day18.cpp : Defines the entry point for the application.
//

#include "day18.h"

using cube = std::vector<std::vector<std::vector<bool>>>;
using point = std::tuple<int, int, int>;

cube readPuzzleInput(std::string filename)
{
	cube puzzleCube;
	std::vector<point> cubePointList;
	
	std::ifstream myFile(filename);
	std::string line;
	std::regex pattern(",");
	int xmin, xmax, ymin,ymax,zmin,zmax;
	xmin = ymin = zmin = std::numeric_limits<int>::max();
	xmax = ymax = zmax = std::numeric_limits<int>::min();

	if (myFile.is_open())
	{
		
		while (getline(myFile, line))
		{
			auto it = std::sregex_token_iterator(line.begin(), line.end(), pattern, -1);

			cubePointList.push_back({ stoi(it->str()), stoi((++it)->str()), stoi((++it)->str()) });

			//find maximum cube boundaries
			if (get<0>(cubePointList.back()) < xmin) xmin = get<0>(cubePointList.back());
			if (get<0>(cubePointList.back()) > xmax) xmax = get<0>(cubePointList.back());
			if (get<1>(cubePointList.back()) < ymin) ymin = get<1>(cubePointList.back());
			if (get<1>(cubePointList.back()) > ymax) ymax = get<1>(cubePointList.back());
			if (get<2>(cubePointList.back()) < zmin) zmin = get<2>(cubePointList.back());
			if (get<2>(cubePointList.back()) > zmax) zmax = get<2>(cubePointList.back());

		}
		myFile.close();
	}
	else
	{
		std::cout << "Cannot read file " << filename << ".\n";
		exit(1);
	}

	//create cube (add one spare point each side)
	int dx = xmax - xmin + 1 + 2,
		dy = ymax - ymin + 1 + 2,
		dz = zmax - zmin + 1 + 2;

	size_t	ix_0 = 1ll-xmin,
			iy_0 = 1ll-ymin,
			iz_0 = 1ll-zmin;

	for (int i = 0; i < dx; ++i)
	{
		puzzleCube.push_back(std::vector < std::vector < bool>>{});
		for (int j = 0; j < dy; ++j)
		{
			puzzleCube.back().push_back(std::vector<bool>(dz, false));
		}
	}

	for (auto [x, y, z] : cubePointList)
	{
		puzzleCube.at(x + ix_0).at(y + iy_0).at(z + iz_0) = true;
	}
	return puzzleCube;
}

void printCube(const cube &puzzleCube)
{

	std::cout << "\n";
	for (auto& x : puzzleCube)
	{
		for (auto& y : x)
		{
			for(auto z: y)
			{
				std::cout << z << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}

int countCubeSides(const cube& puzzleCube)
{
	int sideTileCount = 0;
	for (int i = 0; i< puzzleCube.size();++i)
	{
		for (int j = 0; j < puzzleCube.at(0).size(); ++j)
		{
			for (int k = 0; k < puzzleCube[0][0].size(); ++k)
			{
				//if pixel is occupied look around which sides are not connected
				if (puzzleCube[i][j][k] == 1)
				{
					//get points of all neighbours
					std::vector<point> neighbours = { {i - 1,j,k},{i + 1,j,k}, {i,j - 1,k},{i,j + 1,k},{i,j,k - 1},{i,j,k + 1} };
					for (auto [l, m, n] : neighbours)
					{
						if (puzzleCube[l][m][n] == 0) ++sideTileCount;
					}

				}
				//else do nothing
			}
		}
	}
	return sideTileCount;
}

cube invertCube(const cube& puzzleCube)
{
	cube invCube;

	for (int i = 0; i < puzzleCube.size(); ++i)
	{
		invCube.push_back(std::vector < std::vector<bool>>());
		for (int j = 0; j < puzzleCube.at(0).size(); ++j)
		{
			invCube.back().push_back(std::vector<bool>());
			for (int k = 0; k < puzzleCube[0][0].size(); ++k)
			{
				invCube.back().back().push_back(!puzzleCube[i][j][k]);
			}
		}
	}
	
	return invCube;
}

void resetOuterBound(cube& puzzleCube, int i, int j, int k)
{
	//check if we are still in cube bounds
	if (i < 0 || i >= puzzleCube.size() ||
		j < 0 || j >= puzzleCube.at(i).size() ||
		k < 0 || k >= puzzleCube.at(i).at(j).size())
	{
		return;
	}
	//check if we are still in territory to be reset
	//if yes, reset and recursive call all neighbours
	if (puzzleCube[i][j][k] == 1)
	{
		puzzleCube[i][j][k] = 0;
		resetOuterBound(puzzleCube, i + 1, j, k);
		resetOuterBound(puzzleCube, i - 1, j, k);
		resetOuterBound(puzzleCube, i, j + 1, k);
		resetOuterBound(puzzleCube, i, j - 1, k);
		resetOuterBound(puzzleCube, i, j, k + 1);
		resetOuterBound(puzzleCube, i, j, k - 1);
	}
	return;
}

int countOuterCubeSides(const cube& puzzleCube)
{
	//extract enclosed pixel from puzzle cube
	//1. invert puzzle cube
	cube invCube = invertCube(puzzleCube);
	
	//2. reset parts outside original cubes
	resetOuterBound(invCube, 0, 0, 0);

	//printCube(invCube);
	//printCube(puzzleCube);
	
	//3. count all sides of this new cube
	int numInnerSides = countCubeSides(invCube);

	//4. number of outer sides of puzzle cube is all puzzle cube sides - new cube sides
	int numAllSides = countCubeSides(puzzleCube);

	return numAllSides - numInnerSides;
}

int main()
{
	cube testCube = readPuzzleInput("puzzleTest.txt");
	cube puzzleCube = readPuzzleInput("puzzleInput.txt");
	
	std::cout << "Part 1:\n";
	std::cout << "Unconnected side in test cube: " << countCubeSides(testCube) << "\n";
	std::cout << "Unconnected side in puzzle cube: " << countCubeSides(puzzleCube) << "\n";
	
	std::cout << "Part 2:\n";
	std::cout << "Outer side in test cube: " << countOuterCubeSides(testCube) << "\n";
	std::cout << "Outer side in puzzle cube: " << countOuterCubeSides(puzzleCube) << "\n";

	return 0;
}
