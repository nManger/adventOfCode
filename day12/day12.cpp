// day12.cpp : Defines the entry point for the application.
//

#include "day12.h"

using namespace std;


struct vertex {
	int posX; 
	int posY; //posY is pointing downwards, so is technically -y.
	int height;
	int dist = numeric_limits<int>::max();
	bool destination = false;
	bool checked = false;

	bool operator<(const vertex& other) const
	{
		return tie(posX, posY) < tie(other.posX, other.posY);
	}
};

vector<vertex> preparePuzzleInput(string filename)
{
	vector<vertex> vertexList;

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		string line;
		int x = 0, y = 0;
		while (getline(myfile, line))
		{
			for (char c : line)
			{
				//create vertex and initialize position
				vertex newVertex = {x,y};
				
				//mark starting point with distance=0 and set height to 0 ( === a)
				if (c == 'S') {
					newVertex.dist = 0;
					newVertex.height = 0;
				}
				//mark destination point by setting destination flag and set height to 25 ( === z)
				else if (c == 'E')
				{
					newVertex.destination = true;
					newVertex.height = 'z' - 'a';
				}
				//all other points
				else
				{
					newVertex.height = c - 'a';
				}

				vertexList.push_back(newVertex);
				x++;
			}
			//increase y position and reset x
			y++;
			x = 0;
		}
		myfile.close();
	}
	else
	{
		cout << "Cannot open file" << filename <<".\n";
	}

	return vertexList;
}

//find vertex with shortest vertex.dist not already checked and return its list index
int findNextVertex(const vector<vertex>& vertexList)
{
	int iNextVertex =0;
	while (vertexList.at(iNextVertex).checked){iNextVertex++;} //find first vertex not checked

	for (int i = iNextVertex+1; i < vertexList.size(); i++)
	{
		if (!(vertexList.at(i).checked) && (vertexList.at(i).dist < vertexList.at(iNextVertex).dist))
		{
			iNextVertex = i;
		}
	}

	return iNextVertex;
}

// update neighboring vertices if they exist, have not already been checked
//and can be stepped on as per puzzle part 1 rules
void updateNeighborsPart1(vector<vertex>& vertexList, int iCenterVertex, int dx)
{
	
	//left
	int iNeighbour = iCenterVertex - 1;
	if (iNeighbour >= 0 &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height <= 1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//right
	iNeighbour = iCenterVertex + 1;
	if (iNeighbour < vertexList.size() &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height <= 1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//up
	iNeighbour = iCenterVertex - dx;
	if (iNeighbour >= 0 &&
		!vertexList.at(iNeighbour).checked && 
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height <= 1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//down
	iNeighbour = iCenterVertex + dx;
	if (iNeighbour < vertexList.size() &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height <= 1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}
}

// update neighboring vertices if they exist, have not already been checked
//and can be stepped on as per puzzle part 2 rules (== inverse part 1 rules)
void updateNeighborsPart2(vector<vertex>& vertexList, int iCenterVertex, int dx)
{
	//left
	int iNeighbour = iCenterVertex - 1;
	if (iNeighbour >= 0 &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height >= -1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//right
	iNeighbour = iCenterVertex + 1;
	if (iNeighbour < vertexList.size() &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height >= -1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//up
	iNeighbour = iCenterVertex - dx;
	if (iNeighbour >= 0 &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height >= -1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}

	//down
	iNeighbour = iCenterVertex + dx;
	if (iNeighbour < vertexList.size() &&
		!vertexList.at(iNeighbour).checked &&
		(vertexList.at(iNeighbour).height - vertexList.at(iCenterVertex).height >= -1))
	{
		vertexList.at(iNeighbour).dist = vertexList.at(iCenterVertex).dist + 1;
	}
}


//Dijkstra-algorithm for finding shortest path
int findShortestPath(const vector<vertex>& grid, function<void(vector<vertex>&,int,int)> updateNeighbors)
{
	int numVerticesChecked = 0; 
	vector<vertex> vertexList = grid; //generate a changable copy of the grid
	int iNextVertex=0;
	//find grid width (items are stored height first, so idx = x + y*widthX
	int dX = 0;
	while (vertexList.at(dX).posY == 0) {
		dX++; };

	//iterate unitl all items in vertex list have been checked
	while (numVerticesChecked<vertexList.size())
	{
		//find next vertex and mark it as checked
		iNextVertex = findNextVertex(vertexList);
		vertexList.at(iNextVertex).checked = true;
		numVerticesChecked++;

		//if new vertex is destination vertex, break loop
		if (vertexList.at(iNextVertex).destination) break;

		//update neighbours
		updateNeighbors(vertexList, iNextVertex, dX);
	}

	//return distance of end vertex == last nextVertex
	//cout <<"("<< vertexList.at(iNextVertex).posX << " " << vertexList.at(iNextVertex).posY << ") ";
	return vertexList.at(iNextVertex).dist;
}


//For part 2 we need to redefine our part 1 destination to start
//and all grid points with height=0 as destinations
vector<vertex> modifyGridForPart2(const vector<vertex>& grid)
{
	vector<vertex> newGrid;
	for (vertex vert : grid)
	{
		//reset previous destination (is destination and has elevation 25) to start
		if (vert.height == 25 && vert.destination)
		{
			vert.destination = false;
			vert.dist = 0;
		}
		//set all verts with height 0 as destinations
		else if (vert.height == 0)
		{
			//reset previous start point (has height 0 and dist 0)
			if (vert.dist == 0) vert.dist = numeric_limits<int>::max();
			vert.destination = true;
		}
		//else leave vert unmodified

		newGrid.push_back(vert);
	}
	return newGrid;
}

int main()
{
	vector<vertex> testGrid = preparePuzzleInput("puzzleTest.txt");
	vector<vertex> puzzleGrid = preparePuzzleInput("puzzleInput.txt");

	cout << "Shortest path in test grid is " << findShortestPath(testGrid,updateNeighborsPart1) << ".\n";
	cout << "Shortest path in puzzle grid is " << findShortestPath(puzzleGrid,updateNeighborsPart1) << ".\n";

	cout << "Shortest hiking trail in test grid is " << findShortestPath(modifyGridForPart2(testGrid), updateNeighborsPart2) << ".\n";
	cout << "Shortest hiking trail in puzzle grid is " << findShortestPath(modifyGridForPart2(puzzleGrid), updateNeighborsPart2) << ".\n";
	
	return 0;
}
