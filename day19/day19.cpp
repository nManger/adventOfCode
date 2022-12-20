// day19.cpp : Defines the entry point for the application.
//

#include "day19.h"

//stores cost of each type of robot in array in order ore,clay,obsidian
struct Blueprint
{
	int id = 0;
	std::array<int, 3> oreBot = { 0,0,0 };
	std::array<int, 3> clayBot = { 0,0,0 };
	std::array<int, 3> obsidianBot = { 0,0,0 };
	std::array<int, 3> geodeBot = { 0,0,0 };

	friend std::ostream& operator<< (std::ostream& stream, const Blueprint& blueprint);
};

std::ostream& operator<< (std::ostream& stream, const Blueprint& blueprint)
{
	stream << "Cost ore bot: " << blueprint.oreBot[0] << "," << blueprint.oreBot[0] << "," << blueprint.oreBot[0] << " ";
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
	
	return 0;
}
