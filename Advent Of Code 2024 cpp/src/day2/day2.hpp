#include "./../util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <set>

void day2(std::string_view _input) {
    std::vector<std::string> reports = serializeInput(_input);
	size_t safeCount = 0;

	for (auto& report : reports)
	{
		std::istringstream stream(report);
		int increase = -1;
		for (int reading, prev = -1; stream >> reading; prev = reading)
		{
			if (prev == -1)
			{
				continue;
			}
			if (increase == -1)
			{
				increase = prev < reading;
			}
			int diff = std::abs(prev - reading);
			if (!diff || diff > 3 || prev < reading != increase)
			{
				goto nextReport;
			}
		}
		safeCount++;
	nextReport:;
	}
	std::cout << "Day2: \n \tSafe Count: " << safeCount << "\n";
	
	safeCount = 0;
	for (size_t i = 0; i < reports.size(); i++)
	{
		size_t ignoredIndex = 0;
		tryAgain:
		std::istringstream stream(reports[i]);
		int increase = -1;
		for (int reading, prev = -1, i = 1; stream >> reading; i++)
		{
			if (ignoredIndex == i)
			{
				continue;
			}
			if (prev == -1)
			{
				prev = reading;
				continue;
			}
			if (increase == -1)
			{
				increase = prev < reading;
			}
			int diff = std::abs(prev - reading);
			if (!diff || diff > 3 || prev < reading != increase)
			{
				ignoredIndex++;
				// uglay af, but time is rare :D
				if (ignoredIndex >= reports[i].length())
				{
					goto nextEntry2;
				}
				goto tryAgain;
			}
			prev = reading;
		}
		safeCount++;
	nextEntry2:;
	}
	std::cout << "\n\tDampened Safe Count: " << safeCount << "\n";
}
