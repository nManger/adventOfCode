
#include "day9.h"

using namespace std;


Rope::Rope():numKnots(2)
{
	for (int i = 0; i < numKnots; i++)
	{
		knotPos.push_back({ 0,0 });
	}
	this->prevTailPos.insert(make_tuple(0,0));
}

Rope::Rope(int nKnots) :numKnots(nKnots)
{
	for (int i = 0; i < numKnots; i++)
	{
		knotPos.push_back({ 0,0 });
	}
	this->prevTailPos.insert({ 0,0 });
}

Rope::Rope(int nKnots, const tuple<int, int>& startPos):numKnots(nKnots)
{
	for (int i = 0; i < numKnots; i++)
	{
		knotPos.push_back(startPos);
	}
	this->prevTailPos.insert(startPos);
}

set<tuple<int, int>> Rope::getPrevTailPos()
{
	return prevTailPos;
}

void Rope::move(char direction, int steps)
{
	//cout << direction << "\n";
	for (int i = 0; i < steps; i++)
	{
		moveHead(direction);
		adjustTail();
		prevTailPos.insert(knotPos.back());
		//plotRopePosition();
		//cout << "\n";
	}
}

void Rope::moveHead(char direction)
{

	auto [x, y] = knotPos.at(0);

	switch (direction)
	{
		case 'U':
			knotPos.at(0) = {x + 1,y};
			break;
		case 'D':
			knotPos.at(0) = { x - 1,y };
			break;
		case 'L':
			knotPos.at(0) = { x,y - 1 };
			break;
		case 'R':
			knotPos.at(0) = { x,y + 1 };
			break;
	}
}

void Rope::adjustTail()
{
	for (int i = 1; i < numKnots; i++)
	{
		//treat every 2-knot-link as head-tail pair
		auto [xHead, yHead] = knotPos.at(i-1);
		auto [xTail, yTail] = knotPos.at(i);

		int dx = xHead - xTail;
		int dy = yHead - yTail;

		//tail moves up or down
		if (abs(dx) > 1 && dy == 0)
		{
			knotPos.at(i) = { xTail + sign(dx) , yTail };
		}
		//tail moves left or right
		else if (abs(dy) > 1 && dx == 0)
		{
			knotPos.at(i) = { xTail , yTail + sign(dy) };
		}
		//tail moves diagonally
		else if ((abs(dy) > 1 && dx != 0) || (abs(dx) > 1 && dy != 0))
		{
			knotPos.at(i) = { xTail + sign(dx) , yTail + sign(dy) };
		}
		//no movement of piece i -> no movement of pieces j>i
		else
		{
			break;
		}
	}
}

void Rope::plotTailMovement(int minGridSize)
{
	auto grid = generatePointStack(prevTailPos,minGridSize);

	if (grid.size() > 50 || grid.at(0).size() > 50)
	{
		string filename = "grid_" + to_string(numKnots) + "PointRope.txt";
		ofstream myfile(filename);
		if (myfile.is_open())
		{
			cout << "Writing tail movement grid to file " << filename << "...\n";
			for (vector<char> row : grid)
			{
				for (char c : row)
				{
					myfile << c;
				}
				myfile << "\n";
			}
			myfile.close();
		}
		else
		{
			cout << "Could not write tail movement grid to file.\n";
		}
	}
	else
	{
		for (vector<char> row : grid)
		{
			for (char c : row)
			{
				cout << c;
			}
			cout << "\n";
		}
	}
}

void Rope::plotRopePosition()
{
	set<tuple<int, int>> knots;
	for (auto& position : knotPos)
	{
		knots.insert(position);
	}
	auto grid = generatePointStack(knots,6);

	for (vector<char> row : grid)
	{
		for (char c : row)
		{
			cout << c;
		}
		cout << "\n";
	}
}

vector<vector<char>> Rope::generatePointStack(set<tuple<int,int>> points,int minGridSize)
{
	int xmax = 0, ymax = 0, xmin = 0, ymin = 0;

	//find bounds
	for (auto [x, y] : points)
	{
		if (x > xmax) xmax = x;
		else if (x < xmin) xmin = x;
		
		if (y > ymax) ymax = y;
		else if (y < ymin) ymin = y;
	}

	int dx = xmax - xmin+1;
	int dy = ymax - ymin+1;

	//set min grid size
	if (dx < minGridSize) dx = minGridSize;
	if (dy < minGridSize) dy = minGridSize;

	vector<vector<char>> grid;

	for (int i = 0; i < dx; i++)
	{
		grid.push_back(vector(dy, '.'));
	}

	for (auto [x, y] : points)
	{
		grid[dx-1-(x-xmin)][y - ymin] = '#';
	}

	return grid;
}