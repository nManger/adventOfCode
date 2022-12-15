// day15.cpp : Defines the entry point for the application.
//

#include "day15.h"


//calculate L1 (colloq. Manhattan) distance of two points
int getManhattanDistance(std::tuple<int,int> pos1, std::tuple<int, int> pos2)
{
	return std::abs(get<0>(pos1) - get<0>(pos2)) + std::abs(get<1>(pos1) - get<1>(pos2));
}

std::vector<Sensor> preparePuzzleInput(std::string filename)
{
	std::vector<Sensor> sensorList;

	std::ifstream myFile(filename);
	std::string line;
	std::regex pattern(R"((x|y)=-?\d+)");

	while (getline(myFile, line))
	{
		Sensor currSensor;
		std::sregex_token_iterator it(line.begin(), line.end(), pattern, 0);
		
		//get sensor coordinates
		int x = stoi(it->str().substr(2));
		it++;
		int y = stoi(it->str().substr(2));
		it++;
		currSensor.position = { x,y };
		
		//get beacon coordinates
		x = stoi(it->str().substr(2));
		it++;
		y = stoi(it->str().substr(2));
		it++; // is now end iterator
		currSensor.nearestBeacon = { x,y };

		currSensor.distanceSensorBeacon = getManhattanDistance(currSensor.position, currSensor.nearestBeacon);
		sensorList.push_back(currSensor);
	}

	return sensorList;
}


int checkLineCoverage(const std::vector<Sensor>& sensorList, int lineYPos)
{
	std::set<std::tuple<int, int>> sensorCoveragesInLine;
	std::set<int> uniqueBeaconsInLine;

	//find all beacons and distinct individual sensor coverages in line
	for (Sensor currentSensor : sensorList)
	{
		//check if beacon is in line
		if (get<1>(currentSensor.nearestBeacon) == lineYPos)
		{
			uniqueBeaconsInLine.insert(get<0>(currentSensor.nearestBeacon));
		}

		int dxHalf = currentSensor.distanceSensorBeacon - abs(get<1>(currentSensor.position) - lineYPos);

		//only add if we have coverage
		if (dxHalf >=0) sensorCoveragesInLine.insert({ get<0>(currentSensor.position) - dxHalf, get<0>(currentSensor.position) + dxHalf });

		//std::cout << get<0>(currentSensor.position) << " " << dxHalf << " " << get<0>(currentSensor.position) - dxHalf << " " << get<0>(currentSensor.position) + dxHalf << "\n";
	}

	//initialize previous xend to point left of first xbegin
	int xEndPrev= get<0>(*sensorCoveragesInLine.begin())-1;
	int numPointsCovered=0;

	//coverage stretches are sorted by ascending xbegin, so no occuring voids will be covered by a following item
	for (auto& [xbegin, xend] : sensorCoveragesInLine)
	{
		//current element not overlapping, add whole range
		if (xbegin > xEndPrev)
		{
			numPointsCovered += abs(xend - xbegin) + 1;
			xEndPrev = xend;
		}
		//partial coverage, add non-covered elements
		else
		{
			if (xend > xEndPrev)
			{
				numPointsCovered += abs(xend - xEndPrev); //xEndPrev is already contained, so don't it add again
				xEndPrev = xend;
			}
			//else element is fully covered, nothing to do
		}
		//std::cout << xbegin << " " << xend << " " << numPointsCovered << "\n";
	}

	//std::cout << numPointsCovered << " " << uniqueBeaconsInLine.size() << " ";
	return numPointsCovered - uniqueBeaconsInLine.size();
}

/* first attempt at line coverage function: too slow
int checkLineCoverage(const std::vector<Sensor>& sensorList, int lineYPos)
{
	std::set<int> lineXPosCovered;
	std::set<int> beaconsInLine;

	for (Sensor currentSensor : sensorList)
	{
		//get maximum x-distance of covered points to the left (right) on line
		int dxHalf = currentSensor.distanceSensorBeacon - abs(get<1>(currentSensor.position) - lineYPos);

		//if current beacon is on line, add to beacons list (if not present)
		if (get<1>(currentSensor.nearestBeacon) == lineYPos)
		{
			beaconsInLine.insert(get<0>(currentSensor.nearestBeacon));
		}

		//add all points in [-dxHalf,dxHalf] (relative to sensors x-position) to list if not yet present
		for (int i = -dxHalf; i <= dxHalf; i++)
		{
			lineXPosCovered.insert(i + get<0>(currentSensor.position));
		}
	}
	//std::cout << lineXPosCovered.size() << " " << beaconsInLine.size() << " ";

	// return number of covered points without beacons
	return lineXPosCovered.size() - beaconsInLine.size();
}*/

std::vector<std::tuple<int, int>> findHolesInLineCoverage(const std::vector<Sensor>& sensorList, int lineYPos, std::tuple<int,int> xBounds)
{

	std::set<std::tuple<int, int>> sensorCoveragesInLine;

	//find all distinct individual sensor coverages in line
	for (Sensor currentSensor : sensorList)
	{
		int dxHalf = currentSensor.distanceSensorBeacon - abs(get<1>(currentSensor.position) - lineYPos);

		//only add if we have coverage
		if (dxHalf >= 0) sensorCoveragesInLine.insert({ get<0>(currentSensor.position) - dxHalf, get<0>(currentSensor.position) + dxHalf });

	}

	//initialize previous xend to point left of leftmost search grid point
	int xEndPrev = get<0>(xBounds);
	std::vector<std::tuple<int, int>> gapPoints;

	//coverage stretches are sorted by ascending xbegin, so no occuring voids will be covered by a following item
	for (auto& [xbegin, xend] : sensorCoveragesInLine)
	{
		//current element not overlapping or touching checked space, gap detected
		if (xbegin > xEndPrev+1)
		{
			for (int i = xEndPrev + 1; i < xbegin; i++)
			{
				gapPoints.push_back({ i,lineYPos });
			}
		}
		//else current element (partially) contained or touching, no gap

		//if endpoint of checked space moved, update
		if(xend> xEndPrev) xEndPrev = xend;

		//break once we leave search grid
		if (xend > get<1>(xBounds)) break;
		
	}
	return gapPoints;
}

std::tuple<int, int> findDistressBeacon(const std::vector<Sensor>& sensorList, const std::tuple<std::tuple<int, int>, std::tuple<int, int>>& bounds)
{
	std::vector<std::tuple<int, int>> gapsInLine;
	std::tuple<int, int> distressBeaconPosition;

	auto& [xBounds, yBounds] = bounds;

	for (int i = get<0>(yBounds); i <= get<1>(yBounds); i++)
	{
		gapsInLine = findHolesInLineCoverage(sensorList, i, xBounds);
		
		//there is only one possible position, so break once first gap is found
		if (!gapsInLine.empty())
		{
			distressBeaconPosition = gapsInLine.at(0);
			break;
		}
	}

	/*if (distressBeaconPositions.size() > 1)
	{
		std::cout << "Multiple locations for distress beacon detected, returning first possible location.\n";
	}
	else if (distressBeaconPositions.empty())
	{
		std::cout << "No locations for distress beacon detected.\n";
		return { -1,-1 };
	}*/

	return distressBeaconPosition;
}

int main()
{
	std::vector<Sensor> testSensorList = preparePuzzleInput("puzzleTest.txt");
	std::vector<Sensor> puzzleSensorList = preparePuzzleInput("puzzleInput.txt");

	std::cout << "Part 1:\n";
	int lineYposTest = 10;
	std::cout << "Number of points unable to have a beacon in line "<< lineYposTest <<" (test): " << checkLineCoverage(testSensorList, lineYposTest) << "\n";

	int lineYposPuzzle = 2000000;
	std::cout << "Number of points unable to have a beacon in line " << lineYposPuzzle << " (puzzle): " << checkLineCoverage(puzzleSensorList, lineYposPuzzle)<< "\n";

	std::cout << "\n Part 2: \n";
	int xyMax = 20;
	std::tuple<int, int> beaconPos = findDistressBeacon(testSensorList, { {0,xyMax},{0,xyMax} });
	xyMax = 4000000;
	std::cout << "Distress beacon position (test) is: " << get<0>(beaconPos) << "," << get<1>(beaconPos) << " Its tuning frequency is: " << get<0>(beaconPos) * xyMax + get<1>(beaconPos) << "\n";

	beaconPos = findDistressBeacon(puzzleSensorList, { {0,xyMax},{0,xyMax} });
	xyMax = 4000000;
	std::cout << "Distress beacon position (puzzle) is: " << get<0>(beaconPos) << "," << get<1>(beaconPos) << " Its tuning frequency is: " << ((long long) get<0>(beaconPos)) * xyMax + get<1>(beaconPos) << "\n";

	return 0;
}
