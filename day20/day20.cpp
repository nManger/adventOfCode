// day20.cpp : Defines the entry point for the application.
//

#include "day20.h"


std::vector<long long> readPuzzleInput(std::string filename)
{
	std::vector<long long> numbers;
	
	std::string line;
	std::ifstream myFile(filename);
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			numbers.push_back(stoi(line));
		}
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}
	return numbers;
}

std::vector<long long> mixEncryptedFile(const std::vector<long long>& fileNumbers, int turns=1)
{
	long long nNumbers = fileNumbers.size();
	std::vector<long long> numbers(nNumbers);
	std::vector<long long> numberPositions(nNumbers);
	std::iota(numberPositions.begin(), numberPositions.end(), 0);

	long long num;
	for (int t = 0; t < turns; ++t)
	{
		for (long long i = 0; i < nNumbers; ++i)
		{
			num = fileNumbers[i] % (nNumbers - 1); //if num>nNumbers remove full list walk-throughs

			//positons with number == 0 do not move
			if (num == 0) continue;

			//move number originally at i
			long long oldPos = numberPositions[i];
			long long newPos = oldPos + num;

			//case 1: new position does not wrap around edges
			if (newPos > 0 && newPos < nNumbers)
			{
				//we moved right, decrement all positions in range (oldPos,newPos] by 1
				if (newPos > oldPos)
				{
					for (long long& val : numberPositions)
					{
						if (val <= newPos && val > oldPos)
						{
							val -= 1;
							if (val < 0) val += nNumbers; //check for wrap around 
						}
					}
				}
				//we moved left, increment all positions in range [newPos,oldPos) by 1
				//as we skip any 0 movement items, this will always be newPos < oldPos
				else
				{
					for (long long& val : numberPositions)
					{
						if (val >= newPos && val < oldPos)
						{
							val += 1;
							if (val >= nNumbers) val -= nNumbers;
						}
					}
				}
			}
			//case 2: new position wraps around edges
			else
			{
				//we moved right and wrapped from high end to low end
				//perform left movement equal to -(nNumbers-num-1) steps
				if (newPos >= nNumbers)
				{
					newPos = newPos - (nNumbers - 1); // = oldPos - (nNumbers - num - 1);
					for (long long& val : numberPositions)
					{
						if (val >= newPos && val < oldPos)
						{
							val += 1;
							if (val >= nNumbers) val -= nNumbers;
						}
					}
				}
				//we moved left and wrapped from low end to high end
				//perform right movement equal to nNumbers-1+num steps
				else
				{
					newPos = newPos + (nNumbers - 1); // = oldPos + (nNumbers - 1) + num;
					for (long long& val : numberPositions)
					{
						if (val <= newPos && val > oldPos)
						{
							val -= 1;
							if (val < 0) val += nNumbers; //check for wrap around 
						}
					}
				}

			}

			numberPositions[i] = newPos;
		}
	}

	//build return vector with numbers at new positions
	for (long long i = 0; i < nNumbers; ++i)
	{
		numbers.at(numberPositions[i]) = fileNumbers[i];
	}

	return numbers;
}

long long getGroveCoordinates(const std::vector<long long>& fileNumbers,long long decryptKey=1, int turns=1)
{
	std::vector<long long> originalNumbers = fileNumbers;
	if (decryptKey > 1 || decryptKey < 0)
	{
		for (long long& val : originalNumbers)
		{
			val *= decryptKey;
		}
	}

	std::vector<long long> mixedNumbers = mixEncryptedFile(originalNumbers,turns);

	long long len = fileNumbers.size();
	//find index where value of mixedNumbers is 0
	long long idx0 = 0;
	for (long long val : mixedNumbers)
	{
		if (val == 0)
		{
			break;
		}
		++idx0;
	}
	

	long long idx[3] = { (1000+idx0) % len, (2000+idx0) % len, (3000+idx0) % len };

	return mixedNumbers.at(idx[0]) + mixedNumbers.at(idx[1]) + mixedNumbers.at(idx[2]);
}
int main()
{
	std::vector<long long> testNumbers = readPuzzleInput("puzzleTest.txt");
	std::vector<long long> puzzleNumbers = readPuzzleInput("puzzleInput.txt");

	
	


	std::cout << "Part 1:\n";
	std::cout << "Sum of grove coordinates (test): " << getGroveCoordinates(testNumbers) << "\n";
	auto begin = std::chrono::steady_clock::now();
	std::cout << "Sum of grove coordinates: " << getGroveCoordinates(puzzleNumbers) << "\n";
	auto end = std::chrono::steady_clock::now();
	std::cout << "Runtime = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	long long key = 811589153;
	int turns = 10;
	std::cout << "\nPart 2:\n";
	std::cout << "Sum of grove coordinates (test): " << getGroveCoordinates(testNumbers,key,turns) << "\n";
	begin = std::chrono::steady_clock::now();
	std::cout << "Sum of grove coordinates: " << getGroveCoordinates(puzzleNumbers,key,turns) << "\n";
	end = std::chrono::steady_clock::now();
	std::cout << "Runtime = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	return 0;
}
