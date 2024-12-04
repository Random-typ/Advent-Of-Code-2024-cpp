#include "./../util.h"
#include <string>
#include <iostream>
#include <regex>

void day3(std::string_view _input) {
	std::smatch color_match;
	std::regex r("mul\\((\\d+),(\\d+)\\)|do\\(\\)|don't\\(\\)");
    std::string s(_input);
	
    uint64_t acc = 0, acc2 = 0;
    bool mulEnabled = true;
    for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r), end = std::sregex_iterator(); i != end; i++)
    {
		if (i->str().starts_with('d'))
		{// do or don't
			mulEnabled = i->str() == "do()";
			continue;
		}
		acc += std::stoi((*i)[1]) * std::stoi((*i)[2]);
		if (mulEnabled)
		{
			acc2 += std::stoi((*i)[1]) * std::stoi((*i)[2]);
		}
    }

	std::cout << "Day3: \n \tResult: " << acc << "\n";
	std::cout << "\n\tAccurate Result: " << acc2 << "\n";
}
