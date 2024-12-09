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
	std::cout << "Day6:\n";
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
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

	HANDLE cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &coinfo);
#endif

	while (true) {
#ifdef _WIN32
		SetConsoleCursorPosition(cHandle, { info.dwCursorPosition.X , info.dwCursorPosition.Y });
		if (_animate) {
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
	Position newObstaclePos = {0, 0};
	while (true) {
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
	std::cout << "\n \tDistinct Positions: " << totalDistinctPositions << "\n";
	std::cout << "\tObstacle position count: " << 0 << "\n";
}
