// day21.cpp : Defines the entry point for the application.
//

#include "day21.h"

long long doMathOperation(long long numA, long long numB, char operation)
{
	if (operation == '+')
	{
		return numA + numB;
	}
	else if (operation == '-')
	{
		return numA - numB;
	}
	else if (operation == '*')
	{
		return numA * numB;
	}
	else if (operation == '/')
	{
		return numA / numB;
	}
	
	return 0;
}

//modified brace matching algorithm from day 13 solution
//get position of outermost bracket pair in string
//returns tuple of -1 (== npos) if no match is found
std::tuple<size_t, size_t> findMatchingBracket(std::string substring, size_t pos=0)
{
	int openBrackets = 0;
	size_t n = substring.find("(",pos);

	for (size_t i = n; i < substring.size(); i++)
	{
		if (substring[i] == '(') openBrackets++;
		else if (substring[i] == ')') openBrackets--;

		//we found match
		if (openBrackets == 0) return { n,i };
	}
	return {-1,-1};
}

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

	return doMathOperation(numA, numB, graph.monkeyList.at(monkeyPos).operation);
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

std::string writeEquationFromMonkeyGraph(const MonkeyGraph& graph, int monkeyPos)
{
	//we have no more children, return monkeys number or h for human
	if (graph.monkeyAdjList.at(monkeyPos).empty())
	{
		if (graph.monkeyList.at(monkeyPos).name == "humn")
		{
			return std::string(1, 'h');
		}

		return std::format("{}", graph.monkeyList.at(monkeyPos).number);
	}
	
	std::string equation = "(";
	equation.append(writeEquationFromMonkeyGraph(graph, graph.monkeyAdjList.at(monkeyPos).at(0)));
	equation.push_back(graph.monkeyList.at(monkeyPos).operation);
	equation.append(writeEquationFromMonkeyGraph(graph, graph.monkeyAdjList.at(monkeyPos).at(1)));
	equation.push_back(')');
	
	return equation;
}

//solves math expression in string assuming only one math operation exists in each bracket
long long solveExpression(const std::string &expr)
{
	auto pattern = std::regex("\\([0-9=\\*/+-]+\\)");
	std::string expression = expr;
	std::sregex_token_iterator it;
	std::string result;
	size_t opPos;

	while (expression.find_first_of("()") != std::string::npos)
	{
		it = std::sregex_token_iterator(expression.begin(), expression.end(), pattern, 0);
		result = "";
		size_t substringEndPos = 0;

		//get substring and drop outer braces
		for (; it != std::sregex_token_iterator(); ++it)
		{
			result += expression.substr(substringEndPos, (it->first - expression.begin() - substringEndPos));
			std::string substring = it->str().substr(1, it->length() - 2);
			opPos = substring.find_first_of("+-*/");

			long long num = doMathOperation(stoll(substring.substr(0, opPos)), stoll(substring.substr(opPos + 1)), substring[opPos]);

			result = std::format("{}{}", result, num);
			substringEndPos = it->second - expression.begin();
		}

		result.append(expression.substr(substringEndPos));

		expression = result;
	}
	
	//check if there is a final operation to be done
	opPos = expression.find_first_of("+-*/");
	if ( opPos == std::string::npos || opPos == 0)
	{
		return stoll(expression);
	}
	
	return doMathOperation(stoll(expression.substr(0, opPos)), stoll(expression.substr(opPos + 1)), expression[opPos]);
}

//solves math equation, assumes variable to be solved for is represented by a single letter h and only present on one side of equation
long long solveEquation(const std::string& lhs,const std::string& rhs)
{
	std::string exprA, exprB;

	//assign side with variable to exprA
	if (lhs.find('h') != std::string::npos)
	{
		exprA = lhs;
		exprB = rhs;
	}
	else
	{
		exprB = lhs;
		exprA = rhs;
	}

	long long result = solveExpression(exprB);

	//continue until only variable char remains
	while (exprA != "h")
	{
		auto [bracketExprBegin, bracketExprEnd] = findMatchingBracket(exprA);

		//if we found outer brackets, drop them and continue to next loop cycle
		if (bracketExprBegin == 0 && bracketExprEnd == exprA.size() - 1)
		{
			exprA = exprA.substr(1, exprA.size() - 2);
			continue;
		}

		//test if we have a complex expression without variable as other operand
		//if so, solve expression and insert result in place
		
		//first bracket is other operand, additionally reset bracket positions
		if (std::string subexpr = exprA.substr(bracketExprBegin + 1, bracketExprEnd - bracketExprBegin - 1); subexpr.find('h') == -1)
		{
			long long subResult = solveExpression(subexpr);
			//check if found bracket is at string beginning (in case last expression contains a complex expression as second operand)
			if (bracketExprBegin == 0)
			{
				exprA = std::format("{}{}", subResult, exprA.substr(bracketExprEnd + 1));

			}
			else
			{
				exprA = std::format("{}{}{}",exprA.substr(0,bracketExprBegin), subResult, exprA.substr(bracketExprEnd + 1));
			}
			
			std::tie(bracketExprBegin, bracketExprEnd) = findMatchingBracket(exprA);
		}
		//second bracket is other operand
		else if (size_t brPos = exprA.find('(', bracketExprEnd + 1);brPos != -1)
		{
			long long subResult = solveExpression(exprA.substr(bracketExprEnd + 2));
			exprA = std::format("{}{}",exprA.substr(0,bracketExprEnd+2),subResult);
		}

		//if no brackets have been found, we have arrived at final operation
		//manually assign bracket positions in a way the operations below are valid
		if (bracketExprBegin == -1)
		{
			if (exprA[0] == 'h')
			{
				exprA = " " + exprA; //insert dummy space
				bracketExprBegin = 0;
				bracketExprEnd = 2;
			}
			else
			{
				bracketExprBegin = exprA.size() - 2;
				bracketExprEnd = exprA.size();
			}
		}

		size_t opPos;
		//find next operation
		if (bracketExprBegin > 0) //op in front of brackets
		{
			opPos = exprA.find_first_of("+-*/");
			if (exprA[opPos] == '+')
			{
				result -= stoll(exprA.substr(0, opPos));
			}
			else if (exprA[opPos] == '-')
			{
				result = stoll(exprA.substr(0, opPos)) - result;
			}
			else if (exprA[opPos] == '*')
			{
				result /= stoll(exprA.substr(0, opPos));
			}
			else if (exprA[opPos] == '/')
			{
				result = stoll(exprA.substr(0, opPos)) / result;
			}
			else
			{
				std::cout << "Something went wrong evaluating expression" + exprA + "\n";
				exit(1);
			}
		}
		else //op behind brackets
		{
			opPos = exprA.find_last_of("+-*/");
			if (exprA[opPos] == '+')
			{
				result -= stoll(exprA.substr(opPos+1));
			}
			else if (exprA[opPos] == '-')
			{
				result += stoll(exprA.substr(opPos+1));
			}
			else if (exprA[opPos] == '*')
			{
				result /= stoll(exprA.substr(opPos+1));
			}
			else if (exprA[opPos] == '/')
			{
				result *= stoll(exprA.substr(opPos+1));
			}
			else
			{
				std::cout << "Something went wrong evaluating expression" + exprA + "\n";
				exit(1);
			}
		}

		exprA = exprA.substr(bracketExprBegin + 1, bracketExprEnd - bracketExprBegin - 1);
	}

	return result;
}

long long getHumanNumberFromMonkeyGraphEquation(const MonkeyGraph& inGraph)
{
	MonkeyGraph graph = inGraph;
	std::string equation;

	graph.monkeyList.at(0).operation = '='; //replace roots operation with equal sign

	//get full equation represented by graph and drop outer braces
	equation = writeEquationFromMonkeyGraph(graph, 0);
	equation = equation.substr(1, equation.size() - 2);
	//std::cout << equation << "\n";

	size_t pos = equation.find('=');
	std::string subExpressionA = equation.substr(0, pos);
	std::string subExpressionB = equation.substr(pos+1);

	return solveEquation(subExpressionA,subExpressionB);
}

int main()
{
	auto testGraph = preparePuzzleInput("puzzleTest.txt");
	auto monkeyGraph = preparePuzzleInput("puzzleInput.txt");

	std::cout << "Part 1:\n";
	std::cout << "Monkey root's number is: " << getMonkeyNumber(testGraph, 0) << "\n";
	std::cout << "Monkey root's number is: " << getMonkeyNumber(monkeyGraph, 0) << "\n";

	//testGraph.monkeyList.at(0).operation = '=';
	//getHumanNumberFromMonkeyGraphEquation(testGraph);

	std::cout << "Part 2:\n";
	std::cout << "Human's number is: " << getHumanNumberFromMonkeyGraphEquation(testGraph) << "\n";
	std::cout << "Human's number is: " << getHumanNumberFromMonkeyGraphEquation(monkeyGraph) << "\n";

	return 0;
}
