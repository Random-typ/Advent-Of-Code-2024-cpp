#include "./src/util.h"
#include "./src/day1/day1.hpp"
#include "./src/day2/day2.hpp"
#include "./src/day3/day3.hpp"
#include "./src/day4/day4.hpp"
#include "./src/day5/day5.hpp"
#include "./src/day6/day6.hpp"
#include "./src/day7/day7.hpp"
#include "./src/day8/day8.hpp"

int main()
{
    day1(readInput(1));
    day2(readInput(2));
    day3(readInput(3));
    day4(readInput(4));
    day5(readInput(5));
    //day6(readInput(6), false);
    //day7(readInput(7));
    day8(readInput(8));

    return 0;
}
