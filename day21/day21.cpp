// day21.cpp : Defines the entry point for the application.
//

#include "day21.h"



MonkeyGraph preparePuzzleInput(const std::string filename)
{
	MonkeyGraph graph;

	std::ifstream myFile(filename);
	std::string line;
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			Monkey monkey;
			size_t pos = line.find(":");
			monkey.name = line.substr(0, pos);
			std::string job = line.substr(pos + 2);

			if (isdigit(job[0]) || job[0] == '-')
			{
				monkey.number = stoi(job);
			}
			else
			{
				pos = job.find_first_of("+-/*");
				monkey.operation = job[pos];

				monkey.childA = job.substr(0, pos - 1);
				monkey.childB = job.substr(pos + 2);
			}
			
			//make sure root is always monkey 0
			if (monkey.name == "root" && graph.monkeyList.size() > 0)
			{
				graph.monkeyList.insert(graph.monkeyList.begin(), monkey);
			}
			else
			{
				graph.monkeyList.push_back(monkey);
			}
		}
		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}

	for (auto it = graph.monkeyList.begin(); it != graph.monkeyList.end(); ++it)
	{
		graph.monkeyAdjList.push_back({});
		graph.monkeyLookup.insert({ it->name,it - graph.monkeyList.begin() });

		//monkey has no child monkeys
		if (it->childA.empty()) continue;

		int childA = -1, childB = -1;

		//search monkey list until both child monkeys have been found
		auto jt = graph.monkeyList.begin();
		while (jt != graph.monkeyList.end() && ( childA < 0 || childB < 0))
		{
			if (jt->name == it->childA) childA = jt - graph.monkeyList.begin();
			if (jt->name == it->childB) childB = jt - graph.monkeyList.begin();
			++jt;
		}

		graph.monkeyAdjList.back().push_back(childA);
		graph.monkeyAdjList.back().push_back(childB);
	}

	return graph;
}

long long getMonkeyNumber(const MonkeyGraph& graph, int monkeyPos)
{
	//if monkey has no child monkeys return number
	if (graph.monkeyAdjList.at(monkeyPos).empty())
	{
		return (long long) graph.monkeyList.at(monkeyPos).number;
	}
	
	long long numA = getMonkeyNumber(graph, graph.monkeyLookup.at(graph.monkeyList.at(monkeyPos).childA));
	long long numB = getMonkeyNumber(graph, graph.monkeyLookup.at(graph.monkeyList.at(monkeyPos).childB));

	switch (graph.monkeyList.at(monkeyPos).operation)
	{
	case '+':
		return numA + numB;
	case '-':
		return numA - numB;
	case '*':
		return numA * numB;
	case '/':
		return numA / numB;
	}
	return 0;
}

void MonkeyGraph::writeMonkeyGraph()
{
	for (int i = 0; i < monkeyList.size(); ++i)
	{
		//write monkey
		std::cout << monkeyList[i].name + " " << i;
		
		//write children
		if (!monkeyList[i].childA.empty())
		{
			std::cout << " " + monkeyList[i].childA + " " << monkeyAdjList[i][0] << " " + monkeyList[i].childB + " " << monkeyAdjList[i][1];
		}
				
		std::cout<< "\n";
	}
	return;
}

int main()
{
	auto monkeyGraph = preparePuzzleInput("puzzleInput.txt");

	std::cout << getMonkeyNumber(monkeyGraph, 0);

	return 0;
}
