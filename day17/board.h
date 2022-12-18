#pragma once

#include <vector>
#include <tuple>
#include "tile.h"

#include <iostream>

class TetrisBoard
{
	Tile currentTile;
	std::vector<Tile> tilesOnBoard;
	int boardWidth;
	int maxTileHeight=0;
	
public:

	TetrisBoard();

	int getTileStackHeight();
	std::vector<Tile> getAllTiles();

	void createNextTile();
	bool moveCurrentTile(directions direction);

private:
	bool checkTileCanMove(directions direction);

};