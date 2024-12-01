#include "day_01/day_01.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
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
