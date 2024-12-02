#include "day_01/day_01.h"
#include "day_02/day_02.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    int day;

    if (argc < 2) {
        std::cout << "No day provided" << std::endl;
        return 1;
    } else if (argc < 3) {
        std::cout << "No input file provided" << std::endl;
        return 1;
    }
    day = atoi(argv[1]);

    switch (day) {
    case 1:
        day_01(argv[2]);
        break;
    case 2:
        day_02(argv[2]);
        break;

    default:
        if (day > 0 && day <= 25) {
            std::cout << "Day " << day << " not implemented" << std::endl;
        } else {
            std::cout << "Invalid day. Day must be a number between 1 and 25"
                      << std::endl;
        }
        break;
    };

    return 0;
}
