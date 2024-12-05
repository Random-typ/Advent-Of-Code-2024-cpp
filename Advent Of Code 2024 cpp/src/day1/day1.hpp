#include "./../util.h"
#include <vector>
#include <iostream>
#include <set>

void day1(std::string_view _input) {
    std::vector<std::string> input = serializeInput(_input);
    std::multiset<int64_t> list1, list2;
    for (auto& i : input)
    {
        if (i.empty())
        {
            continue;
        }
        size_t split = i.find_first_not_of("0123456789");
        list1.insert(std::stoi(i.substr(0, split)));
        list2.insert(std::stoi(i.substr(split)));
    }
    int64_t totalDistance = 0;
    for (auto it1 = list1.begin(), it2 = list2.begin(); it1 != list1.end() && it1 != list1.end(); it1++, it2++) {
        totalDistance += std::abs(*it1 - *it2);
    }
    std::cout << "Day1: \n \tTotal Distance: " << totalDistance << "\n";

    size_t similarityScore = 0;
    for (auto& i : list1)
    {
        similarityScore += list2.count(i) * i;
    }
    std::cout << "\tSimilarity Score: " << similarityScore << "\n";
}
