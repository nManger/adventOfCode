// day6.cpp : Defines the entry point for the application.
//

#include "day6.h"

using namespace std;

string readPuzzleFile(string filename)
{
	string message = "";
	
	//check if filename refers to a .txt file
	if (filename.substr(filename.length() - 4, filename.length()) != ".txt")
	{
		cout << "File not a .txt file.\n";
		return message;
	}

	ifstream myfile(filename);

	if(myfile.is_open())
	{
		message.assign(std::istreambuf_iterator<char>(myfile),std::istreambuf_iterator<char>());
		myfile.close();
	}
	else {
		cout << "Could not open " << filename << "\n";
	}

	return message;
}

/* 
* Identify the string position where the marker with size stride ends.
* Returns -1 if marker is not found.
*/
int findMarker(const string& message,int stride)
{
	//create substring 
	string messageTestPiece;
	bool charPairFound = true;

	unsigned long long pos = 0;
	while(charPairFound && ((pos+stride)<message.size()))
	{
		//get new substring to check and reset charPairFound
		messageTestPiece = message.substr(pos, stride);
		charPairFound = false;

		//check current message substring for char pairs
		for (int j=0;j<stride;j++)
		{
			for (int k=j+1;k<stride;k++)
			{
				if (messageTestPiece[j] == messageTestPiece[k])
				{
					charPairFound=true;
					break;
				}
			}

			//if char pair is found move position marker to first char index + 1
			if (charPairFound)
			{
				pos = pos + j + 1;
				break;
			}
		}
	}

	//check if marker was found or end of message was reached and return
	if (!charPairFound) return pos + stride;

	return -1;
}

int findStartOfPacketMarker(const string& message)
{
	return findMarker(message, 4);
}

int findStartOfMessageMarker(const string& message)
{
	return findMarker(message, 14);
}

int main(int argc, char** argv)
{
	string message;

	//check if command line string (test) or text file is to be used
	//if test, make sure test string has been supplied
	if (strcmp(argv[1],"-t")==0 && argc > 2) 
	{
		message = argv[2];
	}
	else if (strcmp(argv[1],"-f")==0 && argc > 2)
	{
		cout << "Reading file " << argv[2] << "\n";
		message = readPuzzleFile(string(argv[2]));
	}

	//check if string was supplied, otherwise terminate.
	if (message.empty()) {
		cout << "No valid arguments supplied. Use one of -f <filename.txt> or -t <input string>.\n";
		return 0;
	}

	cout << "End position of start-of-package marker is " << findStartOfPacketMarker(message) << ".\n";
	cout << "End position of start-of-message marker is " << findStartOfMessageMarker(message) << ".\n";

	return 0;
}

/* possible test strings:				P1	P2
* mjqjpqmgbljsphdztnvjfqwrcgsmlb		7	19
* bvwbjplbgvbhsrlpgdmjqwftvncz			5	23
* nppdvjthqldpwncqszvftbrmjlhg			6	23
* nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg		10	29
* zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw		11	26
*/