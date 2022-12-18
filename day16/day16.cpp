// day16.cpp : Defines the entry point for the application.
//

#include "day16.h"

using network = std::vector<std::vector<std::tuple<int,int>>>;


std::tuple<network,std::vector<int>> preparePuzzleInput(std::string filename)
{
	network caveNetwork;
	std::vector<int> flowRates; //map cave index to flow rate
	std::map<std::string, int> lookupTable; //map cave name to cave index

	std::vector<std::string> input;
	std::string line;
	std::ifstream myFile(filename);
	
	//read whole input file at once
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			size_t it = line.find("Valve AA");
			if (it != std::string::npos) input.insert(input.begin(),line);
			input.push_back(line);
		}
		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " << filename << ".\n";
		exit(1);
	}

	//create lookup table and read flow rates
	int i = 0;
	for (std::string &myline : input)
	{
		//get cave name
		size_t pos = myline.find(" ");
		lookupTable.insert({ myline.substr(pos + 1,2),i });

		//get flow rate
		std::regex pattern = std::regex(R"(\d+)");
		auto it = std::sregex_token_iterator(myline.begin(), myline.end(),pattern, 0);
		flowRates.push_back(stoi((*it).str()));

		i++;
	}

	//create network adjacency list
	for (std::string myline : input)
	{
		std::vector<std::tuple<int,int>> neighbours;
		std::regex pattern = std::regex(R"([A-Z]{2})");
		auto it = std::sregex_token_iterator(myline.begin(), myline.end(), pattern, 0);
		it++; //first occurence is current cave, skip
		for (; it != std::sregex_token_iterator(); it++)
		{
			if (auto x = lookupTable.find((*it).str()); x != lookupTable.end())
			{
				neighbours.push_back({ (*x).second ,1});
			}
		}
		caveNetwork.push_back(neighbours);
	}

	return { caveNetwork,flowRates };
}


int walkToNextCave(const CaveExploration& explorerKit,int location, int timer, int pressure, std::vector<int> path)
{
	int prs, prsMax = 0;
	//continue walking if we have time
	if (timer >0) {

		//get all nodes not yet visited
		std::vector<std::tuple<int, int>> remNodes;
		bool found = false;
		for (auto& branch : explorerKit.caveNetwork.at(location))
		{
			for (int loc : path)
			{
				if (loc == get<0>(branch))
				{
					found = true;
					break;
				}
			}
			if (!found) remNodes.push_back(branch);
			found = false;
		}
		//we've visited all nodes
		if (remNodes.empty())
		{
			return pressure + (timer-1) * explorerKit.flowRates.at(location);
		}
		path.push_back(location);
		if(location != explorerKit.flowRates.size()-1) timer -= 1; //open valve
		for (auto& branch : remNodes)
		{
			prs = pressure + timer * explorerKit.flowRates.at(location);
			prs = walkToNextCave(explorerKit, get<0>(branch), timer - get<1>(branch),prs,path);
			if (prs > prsMax) prsMax = prs;
		}
	}
	//we ran out of time, return 
	else
	{
		return pressure;
	}
	
	return prsMax;
}

std::tuple< network, std::vector<int>>collapseNetwork(const network& caveNetwork, const std::vector<int>& flowRates)
{
	std::vector<std::vector<int>> distanceMatrix;
	std::vector<int> nonZeroFlowNodes;
	network collapsedCaveNetwork;

	int networkSize = caveNetwork.size();
	//initialize distance matrix to max_short ~ inf
	for (int i = 0; i < networkSize;i++)
	{
		distanceMatrix.push_back(std::vector<int>(networkSize, std::numeric_limits<short>::max()));
	}

	//get min distance between all nodes using Floyd-Warshall algorithm
	for (int i = 0; i < networkSize; i++)
	{
		distanceMatrix.at(i).at(i) = 0;
		for (int j = 0; j < caveNetwork.at(i).size(); j++)
		{
			auto& [k, w] = caveNetwork.at(i).at(j);
			distanceMatrix.at(i).at(k) = w;
		}
	}

	for (int k = 0; k < networkSize; k++)
	{
		for (int i = 0; i < networkSize; i++)
		{
			for (int j = 0; j < networkSize; j++)
			{
				int newPath = distanceMatrix.at(i).at(k) + distanceMatrix.at(k).at(j);
				if (distanceMatrix.at(i).at(j) > newPath)
				{
					distanceMatrix.at(i).at(j) = newPath;
					distanceMatrix.at(j).at(i) = newPath;
				}
			}
		}
	}

	//get mapping of old to new node number and flow rates for reduced network
	int i = 0; //new node number
	std::vector<int> oldNewNetworkMap;
	for (int n = 0; n<flowRates.size();n++)
	{
		if (flowRates.at(n) > 0)
		{
			oldNewNetworkMap.push_back(n);
			nonZeroFlowNodes.push_back(flowRates.at(n));
			i++;
		}
	}

	//build new reduced network containing connections between all nodes
	//as walking between two distant nodes over other nodes is like walking through the middle nodes without valve opening
	for (int i = 0; i < oldNewNetworkMap.size(); i++)
	{
		std::vector<std::tuple<int, int>> neighbours;
		for (int j = 0; j < oldNewNetworkMap.size(); j++)
		{
			if (i == j) continue; // do not point to own location
			neighbours.push_back({ j,distanceMatrix.at(oldNewNetworkMap.at(i)).at(oldNewNetworkMap.at(j)) });
		}
		collapsedCaveNetwork.push_back(neighbours);
	}

	//append old starting node and connect it to all nodes in new network
	std::vector < std::tuple<int, int> > neighbours;

	for (int i = 0; i < oldNewNetworkMap.size(); i++)
	{
		neighbours.push_back({ i,distanceMatrix.at(0).at(oldNewNetworkMap.at(i)) });
	}
	collapsedCaveNetwork.push_back(neighbours);
	nonZeroFlowNodes.push_back(0);

	return { collapsedCaveNetwork,nonZeroFlowNodes };
}

int exploreCaves(const network& caveNetwork, const std::vector<int>& flowRates)
{
	auto [collapsedCaveNetwork,collFlowRates] = collapseNetwork(caveNetwork,flowRates);


	CaveExploration explorerKit = { collapsedCaveNetwork,collFlowRates};

	//starting node is last node in network
	return walkToNextCave(explorerKit, collapsedCaveNetwork.size()-1, 30, 0, std::vector<int>());
}

int main()
{
	auto [testCaves, testFlowRates] = preparePuzzleInput("puzzleInput.txt");

	std::cout << exploreCaves(testCaves, testFlowRates);

	return 0;
}

