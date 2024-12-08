#include "./../util.h"

enum class Directions {
	up,
	down,
	left,
	right
};

struct Coordinate {
	uint64_t x;
	uint64_t y;
};

void day6(std::string_view _input) {
	std::vector<std::string> map = serializeInput(_input);
	Directions direction = Directions::up;
	Coordinate guardPos;
	size_t totalDistinctPositions = 0;
	for (uint64_t y = 0; y < map.size(); y++)
	{
		if (size_t pos = map[y].find('^'); pos != std::string::npos)
		{
			guardPos.y = y;
			guardPos.x = pos;
			break;
		}
	}

	if (map[guardPos.y][guardPos.x] != 'X')
	{
		totalDistinctPositions++;
		map[guardPos.y][guardPos.x] = 'X';
	}
	Coordinate newGuardPos = guardPos;
	switch (direction)
	{
	case Directions::up:
		newGuardPos.y--;
		break;
	case Directions::down:
		newGuardPos.y++;
		break;
	case Directions::left:
		newGuardPos.x--;
		break;
	case Directions::right:
		newGuardPos.x++;
		break;
	}
	if (map[newGuardPos.y][newGuardPos.x] == '#')
	{

	}
	map[guardPos.y][guardPos.x] = '^';






	
	std::cout << "Day6: \n \nDistinct Positions: " << 0 << "\n";
	std::cout << "\t" << 0 << "\n";
}
