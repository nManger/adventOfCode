// day22.cpp : Defines the entry point for the application.
//

#include "day22.h"

enum Cardinal{ right,down,left,up, CARDINAL_SIZE};

int modulo(int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}

std::tuple<std::vector<std::string>, std::vector<std::variant<int,char>>> readPuzzleInput(std::string filename)
{
	std::vector<std::string> map;
	std::vector<std::variant<int, char>> directions;

	std::string line;
	std::ifstream myFile(filename);

	if (myFile.is_open())
	{
		//read map part of file
		getline(myFile, line);
		while (!line.empty() && !myFile.eof())
		{
			map.push_back(line);

			getline(myFile, line);
		}

		getline(myFile, line);

		std::regex pattern = std::regex(R"([RL])");

		auto matchIterator = std::sregex_token_iterator(line.begin(), line.end(), pattern, 0);
		auto nonMatchIterator = std::sregex_token_iterator(line.begin(), line.end(), pattern, -1);

		std::variant<int, char> placeholder;
		while (matchIterator != std::sregex_token_iterator())
		{
			placeholder = stoi(nonMatchIterator->str());
			directions.push_back(placeholder);
			placeholder = matchIterator->str()[0];
			directions.push_back(placeholder);
			++nonMatchIterator;
			++matchIterator;
		}
		directions.push_back(stoi(nonMatchIterator->str()));

		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}

	return { map,directions };
}

long long walkMap(const std::vector<std::string>& inMap, const std::vector<std::variant<int, char>>& directions)
{
	int x = 0, //x increases right
		y = 0, //y increases down
		deltaX = 0,
		deltaY = inMap.size();

	Cardinal heading = right;

	// find largest delta x and pad map
	std::vector<std::string> map;
	for (std::string line : inMap)
	{
		if (line.size() > deltaX) deltaX = line.size();
	}
	for (std::string line : inMap)
	{
		map.push_back(line + std::string(deltaX - line.size(), ' '));
	}
	

	//find initial x position
	x = map.at(0).find_first_not_of(' ');

	for (auto& direction : directions)
	{
		//std::cout << x << " " << y << " " << heading << "\n";
		//change heading
		if (std::holds_alternative<char>(direction))
		{
			if (std::get<char>(direction) == 'R')
			{
				heading = (Cardinal) modulo(heading + 1, CARDINAL_SIZE);
			}
			else
			{
				heading = (Cardinal) modulo(heading - 1, CARDINAL_SIZE);
			}
		}
		else
		{
			int dist = std::get<int>(direction);
			int tmp;
			bool stopped = false;

			for (int i = 0; i < dist; ++i)
			{
				if (stopped) break;

				switch (heading)
				{
				case(right):
					if (x + 1 == deltaX || map[y][x + 1] == ' ') tmp = map.at(y).find_first_of(".#"); else tmp = x + 1;
					if (map[y][tmp] == '#') stopped = true; else x = tmp;
					break;
				case(left):
					if (x - 1 < 0 || map[y][x - 1] == ' ') tmp = map.at(y).find_last_of(".#"); else tmp = x - 1;
					if (map[y][tmp] == '#') stopped = true; else x = tmp;
					break;
				case(up):
					if (y - 1 < 0 || map[y - 1][x] == ' ')
					{
						tmp = deltaY - 1;
						while ( map[tmp][x] == ' ')
						{
							--tmp;
						}
					}
					else
					{
						tmp = y - 1;
					}
					if (map[tmp][x] == '#') stopped = true; else y = tmp;
					break;
				case(down):
					if (y + 1 == deltaY ||  map[y + 1][x] == ' ')
					{
						tmp = 0;
						while (map[tmp][x] == ' ')
						{
							++tmp;
						}
					}
					else
					{
						tmp = y + 1;
					}
					if (map[tmp][x] == '#') stopped = true; else y = tmp;
					break;
				}
			}
		}
	}
	
	//std::cout << x << " " << y <<" " << heading << "\n";
	return (1000ll*(y+1) + 4*(x+1) + heading);
}

int main()
{
	auto [testMap, testDirections] = readPuzzleInput("puzzleTest.txt");
	auto [map, directions] = readPuzzleInput("puzzleInput.txt");
	
	std::cout << "Part 1:\n";
	//std::cout << "Final password (test) is: " << walkMap(testMap, testDirections) << "\n";
	std::cout << "Final password (puzzle) is: " << walkMap(map, directions) << "\n";

	
	/*
	for (std::string line : testMap)
	{
		std::cout << line + "\n";
	}
	std::cout << "\n";

	for (auto& move : testDirections)
	{
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, int>) std::cout << arg << "\n";
			else if constexpr (std::is_same_v<T, char>) std::cout << arg << "\n";
			}, move);
	}*/

	return 0;
}
