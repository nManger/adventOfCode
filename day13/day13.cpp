// day13.cpp : Defines the entry point for the application.
//

#include "day13.h"

using namespace std;

using packetPair = tuple<string, string>;

vector<packetPair> preparePuzzleInput(string filename)
{
	vector<packetPair> packetPairList;

	ifstream myFile(filename);

	if (myFile.is_open())
	{
		string packetA,packetB;
		
		while (!myFile.eof())
		{
			getline(myFile, packetA);
			getline(myFile, packetB);
			
			packetPairList.push_back({packetA,packetB});

			//get the empty line separating pairs
			getline(myFile, packetA);
		}
	}
	else
	{
		cout << "Could not open file " << filename << ".\n";
		exit(1);
	}
	return packetPairList;
}

//get position of matching closing bracket in string
//returns -1 if no match is found
int findMatchingBracket(string substring, int bracketPos)
{
	int openBrackets = 0;

	for (int i= bracketPos; i< substring.size();i++)
	{
		if (substring[i] == '[') openBrackets++;
		else if (substring[i] == ']') openBrackets--;

		//we found match
		if (openBrackets == 0) return i;
	}
	return -1;
}

vector<string> deconstructPacketOuterLevel(const string& packet)
{
	int i = 0, j = 0;
	vector<string> subpackets;
	while (i < packet.size())
	{
		j = packet.find_first_of(",[", i);
		if (j == string::npos) j = packet.size();

		if(packet[j] == '[')
		{
			i = j;
			j = findMatchingBracket(packet, i)+1;
		}
		string d = packet.substr(i, j - i);
		subpackets.push_back(d);

		i = j + 1;
	}
	return subpackets;
}

//compares two packets
//returns 0 if packA > packB, 1 if packA < packB and 2 if packA == packB
int comparePackets(const string& packA, const string& packB)
{
	string packetA = packA, packetB = packB;
	vector<string> subPacketsA, subPacketsB;

	// deconstruct packetA
	if (packetA[0] == '[')
	{
		packetA = packetA.substr(1, packetA.size() - 2);
		subPacketsA = deconstructPacketOuterLevel(packetA);
	}
	else
	{
		subPacketsA = { packetA };
	}

	// deconstruct packetB
	if (packetB[0] == '[')
	{
		packetB = packetB.substr(1, packetB.size() - 2);
		subPacketsB = deconstructPacketOuterLevel(packetB);
	}
	else
	{
		subPacketsB = { packetB };
	}

	int a, b;
	for (int i = 0; i < subPacketsA.size(); i++)
	{
		//b ran out of elements first
		if (i == subPacketsB.size()) return false;


		if (subPacketsA.at(i)[0] == '[' || subPacketsB.at(i)[0] == '[')
		{
			int res = comparePackets(subPacketsA.at(i), subPacketsB.at(i));
			
			//if res == 2 continue search
			if (res != 2) return res;
		}
		else
		{
			a = stoi(subPacketsA.at(i));
			b = stoi(subPacketsB.at(i));

			//if a == b, continue search, else return comparison result
			if (a != b) return a < b;
		}
	}

	//check if packetB has leftover items
	if (subPacketsA.size() < subPacketsB.size()) return true;

	//no decision could be reached
	return 2;
}

int findNumberOfCorrectPairs(const vector<packetPair>& pairList)
{
	int correctPairs = 0;
	for (int i=0; i<pairList.size();i++)
	{
		packetPair currPair = pairList.at(i);
		//cout << get<0>(currPair) << " " << get<1>(currPair) << " ";
		if (comparePackets(get<0>(currPair), get<1>(currPair)) == 1)
		{
			correctPairs += i + 1;
		}
		//cout << correctPairs << "\n";
	}
	return correctPairs;
}

vector<string> sortPackets(const vector<string>& packetList)
{
	vector<string> sortlist = packetList;

	for (int i = 1; i < sortlist.size(); i++)
	{
		int j = i - 1;
		while (j >= 0 && comparePackets(sortlist.at(j),sortlist.at(i)) == 0 )
		{
			j--;
		}
		string movePack = sortlist.at(i);
		sortlist.erase(sortlist.begin() + i);
		sortlist.insert(sortlist.begin() +( j + 1), movePack);
	}
	return sortlist;
}

int getDecoderKey(const vector<string>& packetList)
{
	vector<string> sortedPacketList = sortPackets(packetList);

	int foundMarkers = 0;
	int decoderKey = 1;
	vector<string>::iterator it = sortedPacketList.begin();
	while ( it != sortedPacketList.end() && foundMarkers<2)
	{
		if (*it == "[[2]]" || *it == "[[6]]") 
		{
			foundMarkers++;
			decoderKey *= (it - sortedPacketList.begin()+1);
		}
		it++;
	}
	return decoderKey;
}

vector<string> reorganizePacketPairList(const vector<packetPair>& pairList)
{
	vector<string> packetList = { "[[2]]","[[6]]" };
	for (auto& [a, b] : pairList)
	{
		packetList.push_back(a);
		packetList.push_back(b);
	}
	return packetList;
}

int main()
{
	vector<packetPair> testPairList = preparePuzzleInput("puzzleTest.txt");
	vector<packetPair> puzzlePairList = preparePuzzleInput("puzzleInput.txt");

	cout << "Correctly ordered pairs in test list: " << findNumberOfCorrectPairs(testPairList) << "\n";
	cout << "Correctly ordered pairs in puzzle list: " << findNumberOfCorrectPairs(puzzlePairList) << "\n";

	vector<string> testPacketList = reorganizePacketPairList(testPairList);
	vector<string> puzzlePacketList = reorganizePacketPairList(puzzlePairList);

	cout << "The decoder key for the test message is: " << getDecoderKey(testPacketList) << "\n";
	cout << "The decoder key for the puzzle message is: " << getDecoderKey(puzzlePacketList) << "\n";

	return 0;
}
