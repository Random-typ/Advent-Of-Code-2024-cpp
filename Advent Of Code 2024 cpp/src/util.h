#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>

struct Coordinate {
	int64_t x;
	int64_t y;
};

struct Vec2 {
	int64_t x;
	int64_t y;
};

std::string readInput(int _day);

std::vector<std::string> serializeInput(std::string_view _input);