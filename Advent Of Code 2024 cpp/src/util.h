#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>

std::string readInput(int _day);

std::vector<std::string> serializeInput(std::string_view _input);