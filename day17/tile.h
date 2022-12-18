
#pragma once

#include <vector>
#include <tuple>
//#include "board.h"

enum directions
{
	left, right, down
};

enum tileShapes
{
	hBar, cross, invL, vBar, square, SIZE_TILE_SHAPES
};
/*
static tileShapes operator++(tileShapes& shape);
static tileShapes operator++(tileShapes& shape, int);
static tileShapes operator+(const tileShapes& tileA, const tileShapes& tileB);
static tileShapes operator+(const tileShapes& tileA, int inc);
*/

class Tile
{
	std::vector<std::vector<int>> shape; //shapes are stored in (y,x) format
	std::vector<int> position;
	tileShapes tileShape;


public:

	Tile(); //create hBar tile at (0,0)
	Tile(tileShapes tileShape, std::tuple<int, int> position);


	void moveTile(directions direction);
	std::tuple<int, int> getTilePosition();
	std::tuple<int, int> getTileSize();
	tileShapes getTileShape();
	std::vector<std::tuple<int, int>> getTilePiecesPositions();


private:
	void createTileShape();

};