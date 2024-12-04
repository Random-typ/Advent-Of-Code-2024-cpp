#include "./../util.h"


bool matrixMatch(char _chr, const std::vector<std::string>& _matrix, int _x, int _y) {
	if (_x < 0 || _y < 0 ||
		_x >= _matrix.size() ||
		_y >= _matrix[_x].size() ||
		_matrix[_x][_y] != _chr)
	{
		return false;
	}
	return true;
}

bool checkVector(std::string_view _needle, const std::vector<std::string>& _matrix, int _x, int _y, int _xMul, int _yMul) {
	for (size_t i = 1; i < _needle.size() + 1; i++)
	{
		if (!matrixMatch(_needle[i - 1], _matrix, _x + i * _xMul, _y + i * _yMul))
		{
			return false;
		}
	}
	return true;
};


int checkAllDirections(std::string_view _needle, const std::vector<std::string>& _matrix, int _x, int _y) {
	if (_needle.empty() || _matrix[_x][_y] != _needle[0])
	{
		return 0;
	}
	return
		// right
		checkVector(_needle.substr(1), _matrix, _x, _y, 0, 1) +
		// left
		checkVector(_needle.substr(1), _matrix, _x, _y, 0, -1) +
		// top
		checkVector(_needle.substr(1), _matrix, _x, _y, 1, 0) +
		// bottom
		checkVector(_needle.substr(1), _matrix, _x, _y, -1, 0) +
		// diagonal right bottom
		checkVector(_needle.substr(1), _matrix, _x, _y, 1, 1) +
		// diagonal right top
		checkVector(_needle.substr(1), _matrix, _x, _y, 1, -1) +
		// diagonal left bottom
		checkVector(_needle.substr(1), _matrix, _x, _y, -1, 1) +
		// diagonal left top
		checkVector(_needle.substr(1), _matrix, _x, _y, -1, -1);
}

bool checkCrossVector(std::string_view _needle, const std::vector<std::string>& _matrix, int _x, int _y, int _xMul1, int _yMul1, int _xMul2, int _yMul2) {
	return
		(
			checkVector(_needle.substr(std::ceil(_needle.size() / 2.0)), _matrix, _x, _y, _xMul1, _yMul1) &&
			checkVector(_needle.substr(0, _needle.size() / 2), _matrix, _x, _y, _xMul2, _yMul2)
			) || (
			checkVector(_needle.substr(0, _needle.size() / 2), _matrix, _x, _y, _xMul1, _yMul1) &&
			checkVector(_needle.substr(std::ceil(_needle.size() / 2.0)), _matrix, _x, _y, _xMul2, _yMul2)
		);
}


void day4(std::string_view _input) {
	std::vector<std::string> matrix = serializeInput(_input);

	uint64_t needleCount = 0, needle2Count = 0;
	const std::string_view needle = "XMAS";
	const std::string_view needle2 = "MAS";
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			needleCount += checkAllDirections(needle, matrix, i, j);
			//j = 137;
			if (matrix[i][j] == needle2[needle2.size() / 2])
			{
				needle2Count += checkCrossVector(needle2, matrix, i, j, 1, 1, -1, -1) && checkCrossVector(needle2, matrix, i, j, 1, -1, -1, 1);
			}
		}
	}
	std::cout << "Day4: \n \tXMAS Count: " << needleCount << "\n";
	std::cout << "\tX-MAS Count: " << needle2Count << "\n";
}
