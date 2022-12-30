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

long long stackTiles(std::vector<directions> windPattern, long long maxTiles)
{
	TetrisBoard chamber;

	bool tileMoves=false;
	int iWind = 0;
	int lenWindPattern = windPattern.size();
	long long deltaSH=0, deltaTN=0;
	std::map<int,std::vector<std::vector<long long>>> logging;
	Tile currentTile;

	long long i = 0;
	for (; i < maxTiles; i++)
	{
		tileMoves = true;
		currentTile = chamber.getCurrentTile();

		//every time we get a new tile hBar, store the stack height and tile number and map them to the point in the wind pattern
		if (currentTile.getTileShape() == hBar)
		{
			//std::cout << std::format("i {:5} iWind {:5} stackHeight {:5}\n", i, iWind, chamber.getTileStackHeight());
			if (logging.contains(iWind))
			{
				if (int n = logging.at(iWind).size(); n > 1)
				{
					//check for equidistant stack height and tile number
					long long deltaSHA = logging.at(iWind).at(n-1).at(0) - logging.at(iWind).at(n-2).at(0);
					long long deltaSHB = chamber.getTileStackHeight() - logging.at(iWind).at(n-1).at(0);

					long long deltaTNA = logging.at(iWind).at(n-1).at(1) - logging.at(iWind).at(n - 2).at(1);
					long long deltaTNB = i - logging.at(iWind).at(n-1).at(1);

					if (deltaSHA == deltaSHB && deltaTNA == deltaTNB)
					{
						//std::cout << std::format("Repetition confirmed at Tilenr. = {}, iWind {}, stackHeight {}.\n", i, iWind, chamber.getTileStackHeight());
						//std::cout << std::format("Repetition distance: delta_tilenr. = {}, delta_StackHeight = {}\n", deltaTNA, deltaSHA);
						deltaSH = deltaSHA;
						deltaTN = deltaTNA;
						break;
					}

				}
				else
				{
					logging.at(iWind).push_back({ chamber.getTileStackHeight(),i });
				}
			}
			else
			{
				logging.insert({ iWind, {{chamber.getTileStackHeight(),i}} });
			}
			
		}

		while (tileMoves)
		{
			chamber.moveCurrentTile(windPattern.at(iWind));

			tileMoves = chamber.moveCurrentTile(down);
			
			if (iWind == lenWindPattern - 1) 
			{ 
				iWind = 0;
			}
			else ++iWind;
		}
		chamber.createNextTile();
		
	}

	//we found repetition, calculate total stack height by getting number of repetitions and rest height
	if (i < maxTiles)
	{
		long long m = maxTiles - logging.at(iWind).at(0).at(1); //subtract initial tiles not repeating
		long long nrep = m / deltaTN; //get number of repetitions fitting into remaining tiles
		long long rest = m - nrep * deltaTN; //get number of tiles in unfinished repetition

		auto allTiles = chamber.getAllTiles();
		int n = logging.at(iWind).at(0).at(1) + rest;
		int ymax = 0;
		for (int j = 0; j < n; ++j)
		{
			Tile tile = allTiles.at(j);
			auto shape = tile.getTilePiecesPositions();
			for (auto& [xPiece, yPiece] : shape)
			{
				if (yPiece > ymax) ymax = yPiece;
			}

		}
		return (ymax + 1 + nrep * deltaSH);
	}

	//chamber.printBoard();
	

	return chamber.getTileStackHeight()+1;
}

int main()
{
	std::vector<directions> testWindPattern = getWindPatternFromInput("puzzleTest.txt");
	std::vector<directions> puzzleWindPattern = getWindPatternFromInput("puzzleInput.txt");

	std::cout << "Part 1:\n";
	std::cout << "Height of test rock stack at 2022 Tiles"<< stackTiles(testWindPattern, 2022) << "\n";
	std::cout << "Height of puzzle rock stack at 2022 Tiles " << stackTiles(puzzleWindPattern, 2022) << "\n";
	
	std::cout << "\nPart 2:\n";
	std::cout << "Height of test rock stack at 10^12 Tiles" << stackTiles(testWindPattern, 1000000000000) << "\n";
	std::cout << "Height of puzzle rock stack at 10^12 Tiles " << stackTiles(puzzleWindPattern, 1000000000000) << "\n";

	return 0;
}
