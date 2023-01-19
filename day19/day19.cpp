// day19.cpp : Defines the entry point for the application.
//

#include "day19.h"


//compare bot cost to available resources, return false if any cost[i] > resources[i]
bool resourcesAvailableForBot(const std::array<int, 3>& cost, const std::array<int, 4>& resources)
{
	for (int i = 0; i < 3; ++i)
	{
		if (cost[i] > resources[i]) return false;
	}
	return true;
}

//determine which bots we can build with the ressources we have available
//but: if last step did not build a bot, do not build any of the considered bots
//and if we already have more than blueprint.maxBots for this type, do not build another one
//and:
//and: if the resources by this bot cannot contribute to a geode bot ready before end of final step, do not built bot
reverseSet<BotTypes> getListOfBuildableBots(const Blueprint& blueprint, const State& prevState)
{
	reverseSet<BotTypes> possibleTypes = {};

	//check if we haven't hit the bot cap for this bot and if we have the resources to built it, add if true
	//ore 
	if ( prevState.numBots[0] <= blueprint.maxBots[0] && resourcesAvailableForBot(blueprint.oreBot,prevState.resources))
	{
		possibleTypes.insert(BotTypes::ORE_BOT);
	}
	//clay  
	if ( prevState.numBots[1] <= blueprint.maxBots[1] && resourcesAvailableForBot(blueprint.clayBot, prevState.resources))
	{
		possibleTypes.insert(BotTypes::CLAY_BOT);
	}
	//obsidian
	if  (prevState.numBots[2] <= blueprint.maxBots[2] && resourcesAvailableForBot(blueprint.obsidianBot, prevState.resources))
	{
		possibleTypes.insert(BotTypes::OBSIDIAN_BOT);
	}
	//geode
	if ( resourcesAvailableForBot(blueprint.geodeBot, prevState.resources))
	{
		possibleTypes.insert(BotTypes::GEODE_BOT);
	}

	//check if waiting would possibly unlock new bot in future:
	//do this by checking for any bot type if I have the collection bots for all needed resources but not the amount of resources to build it now
	//ore and clay: only check if bot can't already be built now, as we start with one ore bot
	//obsidian: check if we can't build bot now and if we have a clay bot
	//geode: check if we can't build bot now and if we have an obsidian bot
	if (!possibleTypes.contains(BotTypes::ORE_BOT) || !possibleTypes.contains(BotTypes::CLAY_BOT) || (!possibleTypes.contains(BotTypes::OBSIDIAN_BOT) && prevState.numBots[1] > 0) || (!possibleTypes.contains(BotTypes::GEODE_BOT) && prevState.numBots[2] > 0))
	{
		possibleTypes.insert(BotTypes::NO_BOT);
	}

	//if last bot was no bot, prohibit any bot that could have been built then from being built now
	if (prevState.builtBot == BotTypes::NO_BOT)
	{
		for (BotTypes bot : prevState.consideredBots)
		{
			//exclude wait == no_bot from erasure
			if (bot == BotTypes::NO_BOT) continue;

			if (auto i = possibleTypes.find(bot); i != possibleTypes.end())
			{
				possibleTypes.erase(i);
			}
		}
	}

	return possibleTypes;
}

std::array<int, 4> constructBot(const Blueprint& blueprint, const std::array<int, 4>& resources, BotTypes bot)
{
	std::array<int, 4> newResources = resources;
	std::array<int, 3> cost;

	if (bot == BotTypes::ORE_BOT)
	{
		cost = blueprint.oreBot;
	}
	else if (bot == BotTypes::CLAY_BOT)
	{
		cost = blueprint.clayBot;
	}
	else if (bot == BotTypes::OBSIDIAN_BOT)
	{
		cost = blueprint.obsidianBot;
	}
	else if (bot == BotTypes::GEODE_BOT)
	{
		cost = blueprint.geodeBot;
	}
	else //no bot is constructed, so no resources are consumed
	{
		return resources;
	}
	
	for (int i = 0; i < 3; ++i)
	{
		newResources[i] = resources[i] - cost[i];
	}

	return newResources;
}

int simulateStep(const Blueprint& blueprint, const State& prevState)
{
	int timer = prevState.timer - 1;
	//if timer is 1 return immediately with final number of geodes collected (any robot built in that step will not contribute any resources)
	if (timer == 1)
	{
		return prevState.resources[3] + prevState.numBots[3];
	}
	
	//branch pruning:
	//determine if we can beat maxGeodes from lastState if we would built one geode bot each step from now on
	//if not, terminate branch and return 0
	int nBotsMax = prevState.numBots[3]+(timer-1);
	int nGeodesMax = prevState.resources[3] + (nBotsMax * (nBotsMax + 1)) / 2 - (prevState.numBots[3] * (prevState.numBots[3] - 1)) / 2;
	if (nGeodesMax < prevState.maxGeodes) return 0;

	//determine which bots we can built
	reverseSet<BotTypes> botsToConsider = getListOfBuildableBots(blueprint, prevState);

	//gather resources
	std::array<int, 4> resources = prevState.resources;
	for (int i = 0; i < 4; ++i)
	{
		resources[i] += prevState.numBots[i];
	}

	//loop over all considered bots and update maxGeodes
	int maxGeodes = prevState.maxGeodes;
	for (BotTypes bot : botsToConsider)
	{
		//set up final state of this round to be passed along
		State currState = State();
		currState.timer = timer;
		currState.maxGeodes = maxGeodes;
		currState.builtBot = bot;
		currState.consideredBots = botsToConsider;
		currState.resources = constructBot(blueprint, resources, bot);
		currState.numBots = prevState.numBots;
		if (bot != BotTypes::NO_BOT) currState.numBots[static_cast<int> (bot)] += 1;

		//recur to next step and test if we got more geodes
		int numGeodes = simulateStep(blueprint, currState);
		if (numGeodes > maxGeodes) maxGeodes = numGeodes;
	}

	return maxGeodes;
}

void assessBlueprint(Blueprint& blueprint, int time = 24)
{
	State initState = State();
	initState.timer = time+1;

	blueprint.maxGeodes = simulateStep(blueprint, initState);

	
}

long long getQualityLevelSum(std::vector<Blueprint>& blueprintList)
{
	long long qualitySum = 0;

	for (Blueprint& blueprint : blueprintList)
	{
		assessBlueprint(blueprint);
		std::cout << blueprint.id << " " << blueprint.maxGeodes << " " << (long long)blueprint.id * blueprint.maxGeodes << "\n";
		qualitySum += ((long long)blueprint.id*blueprint.maxGeodes);
	}

	return qualitySum;
}

long long getMaxGeodeProduct(std::vector<Blueprint> blueprintList, int iBeg, int iEnd)
{
	long long maxGeodeProduct = 1;

	for (int i = iBeg; i < iEnd; ++i)
	{
		assessBlueprint(blueprintList[i],32);
		maxGeodeProduct *= blueprintList[i].maxGeodes;
	}
	return maxGeodeProduct;
}

std::vector<Blueprint> preparePuzzleInput(std::string filename)
{
	std::vector<Blueprint> blueprintList;

	std::ifstream myFile(filename);
	std::string line;
	std::regex pattern = std::regex(R"(\d+)");
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			std::sregex_token_iterator it = std::sregex_token_iterator(line.begin(), line.end(), pattern, 0);
			
			Blueprint build;
			build.id = stoi(it->str());
			build.oreBot[0] = stoi((++it)->str()); //ore cost
			build.clayBot[0] = stoi((++it)->str()); //ore cost
			build.obsidianBot[0] = stoi((++it)->str()); //ore cost
			build.obsidianBot[1] = stoi((++it)->str()); //clay cost
			build.geodeBot[0] = stoi((++it)->str()); //ore cost
			build.geodeBot[2] = stoi((++it)->str()); //obsidian cost

			//find max number of each bot needed to cover cost of any one bot at a time
			//ore
			build.maxBots[0] = std::max(build.oreBot[0], build.clayBot[0]);
			build.maxBots[0] = std::max(build.maxBots[0], build.obsidianBot[0]);
			build.maxBots[0] = std::max(build.maxBots[0], build.geodeBot[0]);
			//clay
			build.maxBots[1] = build.obsidianBot[1];
			//obsidian
			build.maxBots[2] = build.geodeBot[2];

			blueprintList.push_back(build);

		}
	}
	else
	{
		std::cout << "Cannot read file " + filename + ".\n";
		exit(1);
	}

	return blueprintList;
}

int main()
{
	auto testList = preparePuzzleInput("puzzleInput.txt");

	//std::cout << getQualityLevelSum(testList);
	std::cout << getMaxGeodeProduct(testList, 0, 3);

	return 0;
}
