// day17.cpp : Defines the entry point for the application.
//

#include "day17.h"


std::vector<directions> getWindPatternFromInput(std::string filename)
{
	std::vector<directions> windPattern;

	std::ifstream myFile(filename);

	if (myFile.is_open())
	{
		std::string input;
		getline(myFile,input);
		for (char c : input)
		{
			switch (c)
			{
			case('>'):
				windPattern.push_back(right);
				break;
			case('<'):
				windPattern.push_back(left);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}
	return windPattern;
}

int stackTiles(std::vector<directions> windPattern, int maxTiles)
{
	TetrisBoard chamber;

	bool tileMoves=false;
	int iWind = 0;
	int lenWindPattern = windPattern.size();
	std::vector<std::vector<int>> logging;
	Tile currentTile;

	for (int i = 0; i < maxTiles; i++)
	{
		tileMoves = true;
		//std::cout << i << " ";

		while (tileMoves)
		{
			chamber.moveCurrentTile(windPattern.at(iWind));

			tileMoves = chamber.moveCurrentTile(down);
			
			if (iWind == lenWindPattern - 1) 
			{ iWind = 0; std::cout << "Wind pattern repeats at Tile"<< i 
				<< " Tile shape is: " << chamber.getAllTiles().back().getTileShape()+1 << "\n"; 
			}
			else ++iWind;
		}
		chamber.createNextTile();
		
		/*
		currentTile = chamber.getAllTiles().back();
		auto [x, y] = currentTile.getTilePosition();
		logging.push_back({(int) currentTile.getTileShape(),x,chamber.getTileStackHeight(),i + 1});

		std::vector<std::vector<std::vector<int>>::iterator> candidates;
		bool repetitionFound = false;
		for (auto it = logging.begin(); it != logging.end()-1; ++it)
		{
			if (((*it)[0] == logging.back()[0]) && ((*it)[1] == logging.back()[1]))
			{
				repetitionFound = false;
				for (auto jt = candidates.begin(); jt != candidates.end(); ++jt)
				{
					if ((((*it)[0] == (*(*jt))[0]) && ((*it)[1] == (*(*jt))[1])))
					{
						//check if candidate, current and last element are equidistant
						if (((*it).at(2) - (*(*jt)).at(2) == logging.back().at(2) - (*it).at(2)) &&
							((*it).at(3) - (*(*jt)).at(3) == logging.back().at(3) - (*it).at(3)))
						{
							std::cout << "Found repetition at " << (*jt) - logging.begin() << " " <<
								it - logging.begin() << " " << logging.size()-1 << "\n";
							std::cout << "Found at tile " << (*(*jt))[0] << " " <<
								(*it)[0] << " " << logging.back()[0] << "\n";
							repetitionFound = true;
							break;
						}
					}
				}
				if (!repetitionFound)
				{
					candidates.push_back(it);
				}
				else
				{
					break;
				}
				
			}
			
		}
		if (repetitionFound)
		{
			break;
		}
		*/
	}

	//print board
	/*
	std::vector<Tile> tileStack = chamber.getAllTiles();

	std::vector<std::vector<char>> board;
	for (int i= 0; i<7;i++)
	{
		board.push_back(std::vector<char>(chamber.getTileStackHeight()+1, '.'));
	}

	
	for (auto it = tileStack.rbegin(); it != tileStack.rend(); ++it)
	{
		std::vector<std::tuple<int, int>> piecePos = (*it).getTilePiecesPositions();
		for (auto [x, y] : piecePos)
		{
			board.at(x).at(y) = '#';
		}
	}

	int row = 0;
	//print board
	for (int j = board.at(0).size()-1; j >=0 ; --j)
	{
		row = 0;
		for (int i = 0; i < 7; ++i)
		{
			//if (board.at(i).at(j) == '#') ++row;
			std::cout << board.at(i).at(j);
		}
		//if (row == 7) std::cout << j <<"\n";
		if ((j + 1) % 40 == 0) std::cout << " xxx";
		std::cout << "\n";
	}*/
	

	return chamber.getTileStackHeight()+1;
}

int main()
{
	std::vector<directions> testWindPattern = getWindPatternFromInput("puzzleTest.txt");
	std::vector<directions> puzzleWindPattern = getWindPatternFromInput("puzzleInput.txt");

	stackTiles(testWindPattern, 4000);

	//std::cout << "Height of test rock stack "<< stackTiles(testWindPattern, 2022) << "\n";
	//std::cout << "Height of puzzle rock stack " << stackTiles(puzzleWindPattern, 2022) << "\n";

	return 0;
}
