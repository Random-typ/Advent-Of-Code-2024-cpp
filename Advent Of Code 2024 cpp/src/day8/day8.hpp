#include "./../util.h"


void day8(std::string_view _input) {
	std::vector<std::string> map = serializeInput(_input);
	
	std::multimap<char, Coordinate> antennas;
	for (int64_t y = 0; y < map.size(); y++)
	{
		for (int64_t x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == '.')
			{
				continue;
			}
			antennas.insert({ map[y][x], {x, y} });
		}
	}

	uint64_t locationCount = 0;
	for (auto it = antennas.begin(); it != antennas.end(); it++)
	{
		for (auto temp = it, it2 = ++temp; it2 != antennas.end() && it2->first == it->first; it2++)
		{
			Vec2 vec{ 
				it->second.x - it2->second.x , 
				it->second.y - it2->second.y 
			};
			
			Coordinate checkLoc1{
				it2->second.x - vec.x,
				it2->second.y - vec.y
			};

			Coordinate checkLoc2{
				it->second.x + vec.x,
				it->second.y + vec.y
			};

			if (checkLoc1.y > 0 && checkLoc1.y < map.size() &&
				checkLoc1.x > 0 && checkLoc1.x < map[checkLoc1.y].size() &&
				map[checkLoc1.y][checkLoc1.x] != '#')
			{
				map[checkLoc1.y][checkLoc1.x] = '#';
				locationCount++;
			}
			if (checkLoc2.y > 0 && checkLoc2.y < map.size() &&
				checkLoc2.x > 0 && checkLoc2.x < map[checkLoc2.y].size() &&
				map[checkLoc2.y][checkLoc2.x] != '#')
			{
				map[checkLoc2.y][checkLoc2.x] = '#';
				locationCount++;
			}
		}
	}

	
	std::cout << "\n \tUnique locations: " << locationCount << "\n";
	std::cout << "\tObstacle position count: " << 0 << "\n";
}
