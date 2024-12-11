#include "./../util.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


enum class Directions {
	up = 0,
	right,
	down,
	left
};

struct Coordinate {
	uint64_t x;
	uint64_t y;
};

void day6(std::string_view _input, bool _animate) {
	std::vector<std::string> originMap = serializeInput(_input), map;
	Directions direction = Directions::up;
	Coordinate guardOrigin, guardPos;
	size_t totalDistinctPositions = 0;
	for (uint64_t y = 0; y < originMap.size(); y++)
	{
		if (size_t pos = originMap[y].find('^'); pos != std::string::npos)
		{
			guardOrigin.y = y;
			guardOrigin.x = pos;
			break;
		}
	}
	std::cout << "Day6:\n";
#ifdef _WIN32
	HANDLE cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (_animate) {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		// set console font size
		CONSOLE_FONT_INFOEX coinfo;
		coinfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		coinfo.nFont = 0;
		coinfo.dwFontSize.X = 0;
		coinfo.dwFontSize.Y = 12;
		coinfo.FontFamily = FF_DONTCARE;
		coinfo.FontWeight = 0;
		LPCWCHAR fontName = L"Consolas";
		memcpy_s(coinfo.FaceName, sizeof(coinfo.FaceName), fontName, sizeof(fontName));

		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &coinfo);
	}
#endif
	guardPos = guardOrigin;
	map = originMap;
	while (true) {
#ifdef _WIN32
		if (_animate) {
			SetConsoleCursorPosition(cHandle, { info.dwCursorPosition.X , info.dwCursorPosition.Y });
			for (auto& i : map)
			{
				fwrite(i.data(), i.size(), 1, stdout);
				fwrite("\n", 1, 1, stdout);
			}
		}
#endif
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
		if (newGuardPos.y >= map.size() || newGuardPos.x >= map[newGuardPos.y].size())
		{
			break;
		}

		if (map[newGuardPos.y][newGuardPos.x] == '#')
		{// move 90 deg
			direction = Directions(((int)direction + 1) % 4);
		}
		else {
			guardPos = newGuardPos;
			if (map[guardPos.y][guardPos.x] != 'X')
			{
				map[guardPos.y][guardPos.x] = '^';
			}
		}
	}
	// part 2
	size_t newObstacleCount = 0;
	Coordinate newObstaclePos = {0, 0};
	char guard = '^';
	for (; newObstaclePos.y < map.size();)
	{
		guardPos = guardOrigin;
		map = originMap;
		direction = Directions::up;
		if (map[newObstaclePos.y].size() < newObstaclePos.x ||map[newObstaclePos.y][newObstaclePos.x] != '.')
		{
			if (newObstaclePos.x < map[0].size())
			{
				newObstaclePos.x++;
				continue;
			}
			newObstaclePos.x = 0;
			newObstaclePos.y++;
			continue;
		}
		map[newObstaclePos.y][newObstaclePos.x] = 'O';
		while (true) {
#ifdef _WIN32
			if (_animate)
			{
				SetConsoleCursorPosition(cHandle, { info.dwCursorPosition.X , info.dwCursorPosition.Y });
				for (auto& i : map)
				{
					fwrite(i.data(), i.size(), 1, stdout);
					fwrite("\n", 1, 1, stdout);
				}
			}
#endif
			Coordinate newGuardPos = guardPos;
			switch (direction)
			{
			case Directions::up:
				newGuardPos.y--;
				guard = '^';
				break;
			case Directions::down:
				newGuardPos.y++;
				guard = 'v';
				break;
			case Directions::left:
				newGuardPos.x--;
				guard = '<';
				break;
			case Directions::right:
				newGuardPos.x++;
				guard = '>';
				break;
			}
			if (newGuardPos.y >= map.size() || newGuardPos.x >= map[newGuardPos.y].size())
			{
				break;
			}

			if (map[newGuardPos.y][newGuardPos.x] == '#' || map[newGuardPos.y][newGuardPos.x] == 'O')
			{// move 90 deg
				direction = Directions(((int)direction + 1) % 4);
			}
			else {
				guardPos = newGuardPos;
				if (map[guardPos.y][guardPos.x] == guard)
				{
					newObstacleCount++;
					break;
				}
				map[guardPos.y][guardPos.x] = guard;
			}
		}
		if (newObstaclePos.x < map[0].size())
		{
			newObstaclePos.x++;
			continue;
		}
		newObstaclePos.x = 0;
		newObstaclePos.y++;
	}
	
	std::cout << "\n \tDistinct Positions: " << totalDistinctPositions << "\n";
	std::cout << "\tObstacle position count: " << newObstacleCount << "\n";
}
