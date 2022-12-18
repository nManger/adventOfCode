
#include "tile.h"

Tile::Tile()
{
	position = { 0,0 };
	tileShape = hBar;

	createTileShape();
}

Tile::Tile(tileShapes tileShape, std::tuple<int, int> position)
{
	this->position = {get<0>(position),get<1>(position)};
	this->tileShape = tileShape;

	createTileShape();
}

void Tile::moveTile(directions direction)
{
	switch (direction)
	{
	case left:
		position[0] -= 1;
		break;
	case right:
		position[0] += 1;
		break;
	case down:
		position[1] -= 1;
		break;
	default:
		break;
	}
}

//get position of lower left corner of tile
std::tuple<int, int> Tile::getTilePosition()
{
	return { position[0],position[1] };
}

std::tuple<int, int> Tile::getTileSize()
{
	return { shape.at(0).size(),shape.size() };
}

std::vector < std::tuple<int, int>> Tile::getTilePiecesPositions()
{
	std::vector < std::tuple<int, int>> positions;
	for (int dy = 0; dy < shape.size(); dy++)
	{
		for (int dx = 0; dx < shape.at(0).size(); dx++)
		{
			if (shape[dy][dx] == 1)
			{
				positions.push_back({ position[0] + dx, position[1] + dy });
			}
		}
	}
	return positions;
}

tileShapes Tile::getTileShape()
{
	return tileShape;
}

void Tile::createTileShape()
{
	switch (tileShape)
	{
	case hBar:
		shape = { {1,1,1,1} };
		break;
	case cross:
		shape = {{0,1,0},
				 {1,1,1},
				 {0,1,0} };
		break;
	case invL:
		//looks upside down as (0,0) is lower left
		shape = {{1,1,1},
				 {0,0,1},
				 {0,0,1} };
		break;
	case vBar:
		shape = {{1},
				 {1},
				 {1},
				 {1} };
		break;
	case square:
		shape = {{1,1},
				 {1,1} };
		break;
	default:
		break;
	}
}

/*
static tileShapes operator++(tileShapes& shape)
{
	shape = static_cast<tileShapes> (((int)shape + 1) % SIZE_TILE_SHAPES);
	return shape;
}

static tileShapes operator++(tileShapes& shape , int)
{
	tileShapes temp = shape;
	shape = static_cast<tileShapes> (((int)shape + 1) % SIZE_TILE_SHAPES);
	return temp;
}

static tileShapes operator+(const tileShapes& tileA, const tileShapes& tileB)
{
	return static_cast<tileShapes> (((int) tileA + (int) tileB) % SIZE_TILE_SHAPES);
}

static tileShapes operator+(tileShapes& tileA, int inc)
{
	return static_cast<tileShapes> (((int) tileA + inc) % SIZE_TILE_SHAPES);
}
*/