// day11.cpp : Defines the entry point for the application.
//

#include "day11.h"

using namespace std;

static bool part2 = true;

int main()
{
	int rounds = 10000;

	MonkeyGame<long long> testGame(string("puzzleTest.txt"));
	cout<<"Level of monkey business in test game: "<< testGame.playGame(rounds) <<"\n";
	
	MonkeyGame<long long> puzzleGame(string("puzzleInput.txt"));
	cout << "Level of monkey business in test game: " << puzzleGame.playGame(rounds) << "\n";

	return 0;
}

template<typename T>
Monkey<T>::Monkey(const std::vector<T>& items, std::function<T(T)> operation, T testDivisor, int monkeyTrue, int monkeyFalse)
{
	for (T item : items)
	{
		itemsHeld.push(item);
	}

	this->operation = operation;
	this->testDivisor = testDivisor;
	this->monkeyTrue = monkeyTrue;
	this->monkeyFalse = monkeyFalse;
}

template<typename T>
Monkey<T>::Monkey(const Monkey& other)
{
	if (this != &other)
	{
		itemsHeld = other.itemsHeld;
		operation = other.operation;
		testDivisor = other.testDivisor;
		monkeyTrue = other.monkeyTrue;
		monkeyFalse = other.monkeyFalse;
		numItemsInspected = other.numItemsInspected;
	}
}

template<typename T>
bool Monkey<T>::hasItems()
{
	return !itemsHeld.empty();
}

template<typename T>
tuple<int,T> Monkey<T>::inspectNextItem()
{
	T worryLevel = itemsHeld.front();
	itemsHeld.pop();
	numItemsInspected++;

	worryLevel = operation(worryLevel);

	//switch on/off part 1 extra directives
	if (!part2) worryLevel /= 3;
	
	//taking the modulo with the least common multiple of all monkeys
	//does not influence the following remainder calculation with the current monkeys denominator
	worryLevel = worryLevel % LCM_ALL_MONKEYS;
	if (worryLevel % testDivisor == 0) return { monkeyTrue, worryLevel };

	return { monkeyFalse, worryLevel };
}

template<typename T>
void Monkey<T>::receiveItem(T item)
{
	itemsHeld.push(item);
}
template<typename T>
T Monkey<T>::getNumItemsInspected()
{
	return numItemsInspected;
}

template<typename T>
T MonkeyGame<T>::playGame(int turns)
{
	for (int i = 0; i < turns; i++)
	{
		for (auto& currentMonkey : monkeyList)
		{
			while (currentMonkey.hasItems())
			{
				auto [newMonkey, item] = currentMonkey.inspectNextItem();
				monkeyList.at(newMonkey).receiveItem(item);
			}
		}
		/*
		cout << "\n Round " << i << "\n";
		for (Monkey& currentMonkey : monkeyList)
		{
			int len = currentMonkey.itemsHeld.size();
			for (int i = 0; i < len; i++)
			{
				cout << currentMonkey.itemsHeld.front() << " ";
				currentMonkey.itemsHeld.push(currentMonkey.itemsHeld.front());
				currentMonkey.itemsHeld.pop();
			}
			cout << "\n";
		}
		*/
	}

	vector<T> numItemsInspectedList;
	numItemsInspectedList.push_back(monkeyList.at(0).getNumItemsInspected());

	for (int i=1; i<monkeyList.size();i++)
	{
		T currentNum = monkeyList.at(i).getNumItemsInspected();
		
		int j = 0;
		for (; j < numItemsInspectedList.size(); j++)
		{
			if (currentNum > numItemsInspectedList.at(j)) break;
		}

		if (j < numItemsInspectedList.size()) numItemsInspectedList.insert(numItemsInspectedList.begin() + j, currentNum);
		else numItemsInspectedList.push_back(currentNum);
	}

	/*
	for (int item : numItemsInspectedList)
	{
		cout << item << " ";
	}
	cout << "\n";
	*/

	return numItemsInspectedList.at(0) * numItemsInspectedList.at(1);
}
template<typename T>
MonkeyGame<T>::MonkeyGame(string filename)
{
	LCM_ALL_MONKEYS = 1;

	ifstream myFile(filename);
	if (myFile.is_open())
	{
		string line;

		while (!myFile.eof())
		{
			vector<string> buffer;
			while (getline(myFile, line))
			{
				if (line.empty()) break;
				
				buffer.push_back(line);
			}
			
			//parse item line (second line in buffer)
			vector<T> items;

			//deconstruct line
			line = buffer.at(1);
			size_t pos = line.find(':');
			string subLine = line.substr(pos + 2);
			regex pattern(", ");
			sregex_token_iterator it(subLine.begin(), subLine.end(), pattern, -1);
			//retrieve items
			for (; it != sregex_token_iterator(); it++)
			{
				items.push_back(stoi(it->str()));
			}
			

			//parse operation function line
			function<T(T)> func;
			//deconstruct line
			line = buffer.at(2);
			pos = line.find('=');
			subLine = line.substr(pos + 2);
			pattern = regex(" ");
			it = sregex_token_iterator(subLine.begin(), subLine.end(), pattern, -1);
			vector<string> funcParts = {it,sregex_token_iterator()};
			//build function
			if (funcParts.at(1) == "+")
			{
				if (isdigit(funcParts.at(2).at(0)))
				{
					T second = stoi(funcParts.at(2));
					func = [second](T x) { return x + second; };
				}
				else
				{
					func = [](T x) { return x + x; };
				}
				
			}
			else if (funcParts.at(1) == "*")
			{
				if (isdigit(funcParts.at(2).at(0)))
				{
					T second = stoi(funcParts.at(2));
					func = [second](T x) { return x * second; };
				}
				else
				{
					func = [](T x) { return x * x; };
				}
			}
			
			//get test
			vector<int> testNums;
			line = buffer.at(3)+buffer.at(4)+buffer.at(5);
			pattern = regex(R"([0-9]+)");
			it=sregex_token_iterator(line.begin(), line.end(), pattern, 0);
			for( ;it!=sregex_token_iterator();it++)
			{
				testNums.push_back(stoi(it->str()));
			}

			LCM_ALL_MONKEYS *= testNums.at(0);
			Monkey<T> newMonkey(items,func,testNums.at(0),testNums.at(1),testNums.at(2));
			monkeyList.push_back(newMonkey);
		}
	}
	else
	{
		cout << "Could not open file";
	}

}
