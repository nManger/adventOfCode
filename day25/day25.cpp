// day25.cpp : Defines the entry point for the application.
//

#include "day25.h"


std::vector<std::string> readPuzzleInput(std::string filename)
{
	std::vector<std::string> inputNumbers;

	std::ifstream myFile(filename);
	std::string line;

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			inputNumbers.push_back(line);
		}
		myFile.close();
	}
	else
	{
		std::cout << "Could not open file " + filename + ".\n";
		exit(1);
	}
	return inputNumbers;
}

long long convertSNAFUToDecimal(std::string snafuNumber)
{
	long long decNumber = 0;

	for (auto it = snafuNumber.rbegin(); it != snafuNumber.rend(); ++it)
	{
		int power = it - snafuNumber.rbegin();
		int num = 0;
		//convert char digit to int
		if (*it == '=') num = -2;
		else if (*it == '-') num = -1;
		else num = *it - '0';

		decNumber += num * (long long)pow(5, power);
	}

	return decNumber;
}

std::string convertDecimalToSNAFU(long long decNumber)
{
	int exp = 0;
	//find highest exponent
	while (decNumber > std::pow(5, exp+1)/2.) //will always work as 5^n/2.0 is always non-integer
	{
		exp++;
	}

	std::queue<int> base5Digits;
	int sign = 1;
	long long rest = decNumber;

	while (exp > 0)
	{
		//get the largest number representable with exp-1 digits
		long long digitFloor = (long long) std::floor(std::pow(5, exp) / 2.);

		//if rest < 0, flip sign and sign indicator
		if (rest < 0)
		{
			rest *= -1;
			sign *= -1;
		}

		int currentDigit = 0;
		//increase current digit until we can represent rest with exp-1 digits
		while (rest > digitFloor)
		{
			rest -= std::pow(5, exp);
			++currentDigit;
		}

		base5Digits.push(sign * currentDigit);
		--exp;
	}

	//rest*sign now contains exp=0 digit
	base5Digits.push(sign * rest);

	//convert digits to snafu notation
	std::string snafuNumber = "";
	int digit;
	char c;
	while (!base5Digits.empty())
	{
		digit = base5Digits.front();
		base5Digits.pop();

		if (digit == -2) c = '=';
		else if (digit == -1) c = '-';
		else c = '0' + digit;

		snafuNumber.push_back(c);
	}

	return snafuNumber;
}

void testEncoders(std::vector<std::string> input)
{
	for (std::string s : input)
	{
		long long num = convertSNAFUToDecimal(s);
		std::cout << std::format("{:>10}", s) + " " + std::format("{:10d}", num) + std::format("{:>10}", convertDecimalToSNAFU(num)) + "\n";
	}
}

std::string getTotalFuelAmountRequired(std::vector<std::string> fuelRequirementList)
{
	long long totalFuel = 0;

	for (std::string amount : fuelRequirementList)
	{
		long long x = convertSNAFUToDecimal(amount);
		totalFuel += x;
	}

	return convertDecimalToSNAFU(totalFuel);
}

int main()
{
	std::vector<std::string> testInput = readPuzzleInput("puzzleTest.txt");
	std::vector<std::string> puzzleInput = readPuzzleInput("puzzleInput.txt");

	std::cout << "Part 1:\n";
	std::cout << "Total fuel required by all balloons (test set): " << getTotalFuelAmountRequired(testInput) << " .\n";
	std::cout << "Total fuel required by all balloons (puzzle set): " << getTotalFuelAmountRequired(puzzleInput) << " .\n";
	
	//testEncoders(testInput);

	return 0;
}
