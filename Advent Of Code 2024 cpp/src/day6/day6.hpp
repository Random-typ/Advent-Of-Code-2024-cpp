#include "./../util.h"

int64_t compute(int64_t _a, int64_t _b, char _operator) {
	switch (_operator) {
	case '-':
		return _a - _b;
	case '+':
		return _a + _b;
	case '*':
		return _a * _b;
	case '/':
		return _a / _b;
	case '|':
		return std::stoll(std::to_string(_a) + std::to_string(_b));
	}
}

uint64_t day7Compute(std::vector<std::string> _calibrations, std::string_view _operators) {

	std::smatch color_match;
	std::regex r("(\\d*)");

	uint64_t sum = 0;
	for (size_t i = 0; i < _calibrations.size(); i++)
	{
		std::cout << "\033[2K\r\tProgress: " << double(i) / _calibrations.size() * 100 << "%";
		std::vector<int64_t> numbers;
		for (std::sregex_iterator j = std::sregex_iterator(_calibrations[i].begin(), _calibrations[i].end(), r), end = std::sregex_iterator(); j != end; j++)
		{
			if (!j->str().empty())
			{
				numbers.emplace_back(std::stoll(j->str()));
			}
		}
		if (numbers.empty())
		{
			continue;
		}
		for (int64_t i = 0; i <= pow(_operators.size(), numbers.size() - 1); i++)
		{
			int64_t result = numbers[1];
			for (size_t pos = 2, j = i; pos < numbers.size(); pos++)
			{
				result = compute(result, numbers[pos], _operators[j % _operators.size()]);
				j /= _operators.size();
			}
			if (result == numbers[0])
			{
				sum += numbers[0];
				break;
			}
		}
	}
	return sum;
}

void day7(std::string_view _input) {
	std::vector<std::string> calibrations = serializeInput(_input), map;
	std::cout << "Day7:\n";

	const std::string_view operators1 = "-+*/";
	const std::string_view operators2 = "-+*/|";
	uint64_t sum = day7Compute({}, operators1);
	uint64_t sum2 = day7Compute(calibrations, operators2);
	
	std::cout << "\tTotal Calibration Result: " << sum << "\n";
	std::cout << "\tTotal Calibration Result2: " << sum2 << "\n";
}
