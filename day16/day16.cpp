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

std::vector<std::vector<int>>permutations(int k, std::vector<int> &A)
{
	if (k == 1) return { std::vector(A) };
	
	std::vector<std::vector<int>> res;
	std::vector<std::vector<int>> subRes;

	subRes = permutations(k-1,A);
	res.insert(res.end(), subRes.begin(), subRes.end());
	
	for (int i = 0; i < k - 1; ++i)
	{
		int dummy;
		//swap A_i <-> A_(k-1)
		if (k % 2 == 0)
		{
			dummy = A[i];
			A[i] = A[k - 1];
			A[k - 1] = dummy;
		}
		//swap A_0 <-> A_(k-1)
		else
		{
			dummy = A[0];
			A[0] = A[k - 1];
			A[k - 1] = dummy;
		}
		subRes = permutations(k - 1, A);
		res.insert(res.end(), subRes.begin(), subRes.end());
	}
	return res;
}

///recursive walk to next cave
///parameters: 
///		explorerKit = cave exploration object containing network information
///		remainingNodes = vector of unexplored nodeIDs
///		node = vector ofcurrent nodeIDs of all players 
///		timer = remaining time
///		pressure = sum of all pressure released by previously opened valves over their respective total open time
///		maxPressure = maximum pressure value found before
///		playerRemDistance = vector of remaining distances of all players incl. valve opening time, players with entry 0 will move during this step
int walkToNextCave(const CaveExploration& explorerKit, std::vector<int> remainingNodes, std::vector<int> node, int timer, int pressure = 0, int maxPressure=0, std::vector<int> playerRemDistance = {0})
{
	//continue walking if we have time
	if (timer >0) {

		int movingPlayers = 0;
		//find distances to remaining nodes for players with dist=0 and update pressure
		std::vector<std::map<int,int>> distances;//map of nodeID in remainingNodes to corresponding distance for each moving player
		for (int i = 0; i < node.size(); ++i)
		{
			if (playerRemDistance.at(i) == 0)
			{
				distances.push_back(std::map<int,int>());
				
				//if we are not in start node: players with dist=0 open their valve in this step
				if (node[0] != explorerKit.flowRates.size() - 1)
				{
					pressure += timer * explorerKit.flowRates.at(node[i]);
				}
				for (auto jt = remainingNodes.begin(); jt != remainingNodes.end(); ++jt)
				{
					for (auto [nodeID, dist] : explorerKit.caveNetwork.at(node[i]))
					{
						if (nodeID == *jt) distances.at(movingPlayers).insert({ nodeID,dist });
					}
				}
				++movingPlayers;
			}
		}

		//if we have no more nodes to walk into
		if (remainingNodes.empty())
		{
			//skip ahead and finish all other players walks
			if (node.size() > 1)
			{
				for (int i = 0; i < node.size(); ++i)
				{
					if (playerRemDistance[i] > 0)
					{
						pressure += (timer-playerRemDistance[i])* explorerKit.flowRates.at(node[i]);
					}
				}
			}
			return pressure;
		}

		//otherwise: check all possible remaining nodes and return maximum relased pressure of all alternatives

		//build an estimator function based on the remaining distances
		//assume you could open all remaining valves this instance: if the total pressure is still smaller than maxPressure, we can never reach maxPressure in this branch, so stop trying
		int testPrs = pressure;
		for (int id : remainingNodes)
		{
			testPrs += (explorerKit.flowRates[id] * (timer-1));
		}
		if (node.size() > 1)
		{
			for (int i = 0; i < node.size(); ++i)
			{
				if (playerRemDistance[i] > 0)
				{
					testPrs += (timer - playerRemDistance[i]) * explorerKit.flowRates.at(node[i]);
				}
			}
		}
		if (testPrs < maxPressure) return 0;

		//get all combinations of remaining nodes to walk to for all moving players (max 2 players)
		std::vector<std::vector<int>> permList;
		if (movingPlayers == 1)
		{
			for (int id : remainingNodes)
			{
				permList.push_back({ id });
			}
		}
		else
		{
			for (int i = 0; i < remainingNodes.size(); ++i)
			{
				for (int j = 0; j < remainingNodes.size(); ++j)
				{
					if (i == j) continue;
					//if we are in the start node, we only need all combinations, not all permutations
					if (node[0] == explorerKit.flowRates.size() - 1 && i > j) continue;
					permList.push_back({ remainingNodes.at(i),remainingNodes.at(j) });
				}
			}
		}

		//go through all found permutations
		while(!permList.empty())
		{
			//get current permutation
			std::vector perm = permList.back();
			permList.pop_back();

			//get new remaining nodes without all nodes to be walked to
			std::vector<int> remNodes;
			for (int otherNode : remainingNodes)
			{
				bool nodeFound = false;
				for (int newNode : perm)
				{
					if (otherNode == newNode)
					{
						nodeFound = true;
						break;
					}
				}
				if (!nodeFound) remNodes.push_back(otherNode);
			}

			//get new distances for players, find min distance and set new destination nodes for players with dist=0
			int n = 0, steps=std::numeric_limits<int>::max(), prs;
			std::vector<int> playerDist;

			for (int i = 0; i < playerRemDistance.size(); ++i)
			{
				int dist = playerRemDistance[i];
				if (dist == 0)
				{
					node.at(i) = perm[n];
					dist = distances.at(n).at(node.at(i))+1; //walking distance + valve opening
					++n;
				}
				playerDist.push_back(dist);
				if ( dist < steps) steps = dist;
			}

			//decrease distances for next step
			for (int i = 0; i < playerDist.size(); ++i)
			{
				playerDist[i] -= steps;
			}

			prs = walkToNextCave(explorerKit, remNodes, node, timer-steps, pressure, maxPressure, playerDist);
			/*std::cout << std::format("{:5d} {:5d} {:5d} {:5d} ",prs,pressure,maxPressure,timer);
			for (int id : node)
			{
				std::cout << id << " ";
			}
			
			std::cout << "\n";*/
			if (prs > maxPressure) maxPressure = prs;
		}
		return maxPressure;
	}
	//we ran out of time, return input pressure
	else
	{
		return pressure;
	}
	
}

std::tuple< network, std::vector<int>>collapseNetwork(const network& caveNetwork, const std::vector<int>& flowRates)
{
	std::vector<std::vector<int>> distanceMatrix;
	std::vector<int> nonZeroFlowNodes;
	network collapsedCaveNetwork;

	size_t networkSize = caveNetwork.size();
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
	//int i = 0; //new node number
	std::vector<int> oldNewNetworkMap;
	for (int n = 0; n<flowRates.size();n++)
	{
		if (flowRates.at(n) > 0)
		{
			oldNewNetworkMap.push_back(n);
			nonZeroFlowNodes.push_back(flowRates.at(n));
			//i++;
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

	//print new network
	/*for (int i = 0; i < oldNewNetworkMap.size(); ++i)
	{
		std::cout << i << " " << oldNewNetworkMap.at(i) << "\n";
	}
	std::cout << "\n";*/

	return { collapsedCaveNetwork,nonZeroFlowNodes };
}

int exploreCaves(const network& caveNetwork, const std::vector<int>& flowRates, int players=1, int timer=30)
{
	if (players > 2)
	{
		std::cout << "Cannot accomodate more than 2 players at this time.\n";
		exit(1);
	}

	auto [collapsedCaveNetwork,collFlowRates] = collapseNetwork(caveNetwork,flowRates);

	CaveExploration explorerKit = { collapsedCaveNetwork,collFlowRates};
	std::vector<int> remNodes = std::vector<int>(collFlowRates.size()-1);
	std::iota(remNodes.begin(), remNodes.end(), 0);

	//starting node is last node in network
	return walkToNextCave(explorerKit, remNodes, std::vector<int>(players,collapsedCaveNetwork.size()-1), timer, 0, 0, std::vector<int>(players,0));
}

int main()
{
	auto [testCaves, testFlowRates] = preparePuzzleInput("puzzleInput.txt");

	std::cout << exploreCaves(testCaves, testFlowRates,2,26);
	

	return 0;
}

