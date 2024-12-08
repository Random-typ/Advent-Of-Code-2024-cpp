#include "./../util.h"
#include <map>
#include <set>
#include <regex>

void day5(std::string_view _input) {
	std::vector<std::string> updates = serializeInput(_input);
	std::map<uint64_t, std::set<uint64_t>> orderingRules;

	uint64_t acc = 0;
	bool orderPart = false;
	for (auto& i : updates)
	{
		if (i.empty())
		{
			orderPart = true;
			continue;
		}
		if (orderPart)
		{
			std::regex r("\\d+.");
			std::vector<int64_t> currentOrder;
			for (std::sregex_iterator j = std::sregex_iterator(i.begin(), i.end(), r), end = std::sregex_iterator(); j != end; j++)
			{
				currentOrder.emplace_back(std::stoll(j->str()));
			}
			for (size_t i = 0; i < currentOrder.size(); i++)
			{
				if (std::any_of(currentOrder.begin(), currentOrder.begin() + i, [&](int64_t v) {
					return orderingRules[currentOrder[i]].contains(v);
					}))
				{// wrong order
					goto nextUpdate;
				}
			}
			acc += currentOrder[currentOrder.size() / 2];
		nextUpdate:
			continue;
		}
		size_t split = i.find('|');
		uint64_t first = std::stoi(i.substr(0, split));
		uint64_t second = std::stoi(i.substr(split + 1));
		orderingRules[first].insert(second);
	}
	// part 2
	orderPart = false;
	uint64_t totalAcc = 0;
	for (auto& i : updates)
	{
		if (i.empty())
		{
			orderPart = true;
			continue;
		}
		if (orderPart)
		{
			std::regex r("\\d+.");
			std::vector<int64_t> currentOrder;
			for (std::sregex_iterator j = std::sregex_iterator(i.begin(), i.end(), r), end = std::sregex_iterator(); j != end; j++)
			{
				currentOrder.emplace_back(std::stoll(j->str()));
			}
			for (size_t j = 0; j < currentOrder.size(); j++)
			{
				for (auto it = currentOrder.begin() + j + 1; it < currentOrder.end(); it++)
				{
					if (orderingRules[currentOrder[j]].contains(*it))
					{
						std::swap(currentOrder[j], *it);
					}
				}
			}
			totalAcc += currentOrder[currentOrder.size() / 2];
		}
	}

	
	std::cout << "Day5: \n \tMiddle Page Score: " << acc << "\n";
	std::cout << "\tIncorrectly-ordered Scores: " << (totalAcc - acc) << "\n";
}
