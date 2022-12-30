
#include "board.h"

TetrisBoard::TetrisBoard()
{
	boardWidth = 7;
	currentTile = Tile(hBar, {2,3});
}

int TetrisBoard::getTileStackHeight()
{
	return maxTileHeight;
}

std::vector<Tile> TetrisBoard::getAllTiles()
{
	return tilesOnBoard;
}

Tile TetrisBoard::getCurrentTile()
{
	return Tile(currentTile);
}

void TetrisBoard::printBoard()
{
	

	std::vector<std::vector<char>> board;
	for (int i= 0; i<7;i++)
	{
		board.push_back(std::vector<char>(maxTileHeight+1, '.'));
	}


	for (auto it = tilesOnBoard.begin(); it != tilesOnBoard.end(); ++it)
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
	}
}

void TetrisBoard::createNextTile()
{
	//put old resting tile on stack
	tilesOnBoard.push_back(currentTile);
	//get new stack height
	auto [x, y] = currentTile.getTilePosition();
	auto [dx, dy] = currentTile.getTileSize();
	if (y + dy - 1 > maxTileHeight) maxTileHeight = y + dy - 1;

	//create new tile as current tile
	tileShapes newShape = static_cast<tileShapes> (((int)currentTile.getTileShape() + 1)% SIZE_TILE_SHAPES);
	currentTile = Tile(newShape, {2,maxTileHeight+4});
}

bool TetrisBoard::moveCurrentTile(directions direction)
{
	if(checkTileCanMove(direction))
	{
		currentTile.moveTile(direction);
		return true;
	}
	return false;
}

bool TetrisBoard::checkTileCanMove(directions direction)
{
	auto [x, y] = currentTile.getTilePosition();
	auto [dx, dy] = currentTile.getTileSize();
	int deltaX = 0, deltaY = 0;

	//simulate new tile position
	switch (direction)
	{
	case left:
		x -= 1;
		deltaX = -1;
		if (x < 0) return false; //tile would move into left wall
		break;
	case right:
		x += 1;
		deltaX = 1;
		if ((x + dx - 1) >= boardWidth) return false; //tile would move into right wall
		break;
	case down:
		y -= 1;
		deltaY = -1;
		if (y < 0) return false; //tile would fall through board floor
		if (y > maxTileHeight) return true; //uppermost resting tile is still completely below 
		break;
	default:
		break;
	}

	//check if we hit any tiles (last tile first as we fall from top down)
	auto currentTilePieces = currentTile.getTilePiecesPositions();
	for (auto it = currentTilePieces.begin(); it != currentTilePieces.end(); ++it)
	{
		*it = { get<0>((*it)) + deltaX , get<1>((*it))+deltaY };
	}
	
	for (auto it = tilesOnBoard.rbegin(); it!= tilesOnBoard.rend(); ++it )
	{
		auto [xOther,yOther] = it->getTilePosition();
		auto [dxOther, dyOther] = it->getTileSize();

		//if other tile is wholly above current tile, we can skip checking it
		if (y+dy-1 < yOther)
		{
			continue;
		}

		//if other tile is wholly below current tile, we can skip it as well
		if (y > (yOther + dyOther - 1))
		{
			continue;
		}

		auto otherTilePieces = it->getTilePiecesPositions();

		for (auto jt = otherTilePieces.begin(); jt != otherTilePieces.end(); ++jt)
		{
			for (auto kt = currentTilePieces.begin(); kt != currentTilePieces.end(); ++kt)
			{
				//piece jt and kt occupy same position -> we would move into tile it
				if ((*kt) == (*jt))
				{
					return false;
				}
			}
			
		}
		
	}
	return true; //we should never end up here
}
