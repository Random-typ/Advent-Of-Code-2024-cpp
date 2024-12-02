#include "util.h"

std::string readInput(int _day) {
    std::fstream fs("./src/day" + std::to_string(_day) + "/input.txt");
    if (!fs.is_open())
    {
        std::cerr << "Failed to open input for day " + std::to_string(_day) + ".\n";
    }
    return std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
}

std::vector<std::string> serializeInput(std::string_view _input) {
    std::vector<std::string> out;
    for (size_t start = 0, end = _input.find('\n', start); start != std::string::npos; start = end, end = _input.find('\n', start + 1)) {
        if (start)
        {
            start++;
        }
        std::string_view line = _input.substr(start, end - start);
        if (!line.empty())
        {
            out.emplace_back(line);
        }
    }
    return out;
}
