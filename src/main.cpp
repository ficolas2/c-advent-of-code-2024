#include "day_01/day_01.h"
#include "day_02/day_02.h"
#include "day_03/day_03.h"
#include "day_04/day_04.h"
#include "day_05/day_05.h"
#include "day_06/day_06.h"
#include "day_07/day_07.h"
#include "day_08/day_08.h"
#include "day_09/day_09.h"
#include "day_10/day_10.h"
#include "day_11/day_11.h"
#include "day_12/day_12.h"
#include "day_13/day_13.h"
#include "day_14/day_14.h"
#include "day_15/day_15.h"
#include "day_16/day_16.h"
#include "day_17/day_17.h"
#include "day_18/day_18.h"
#include "day_19/day_19.h"
#include "day_20/day_20.h"
#include "day_21/day_21.h"
#include "day_22/day_22.h"

#include <cstdlib>
#include <iostream>

void (*const days[])(const char*) = { day_01, day_02, day_03, day_04, day_05,
    day_06, day_07, day_08, day_09, day_10, day_11, day_12, day_13, day_14,
    day_15, day_16, day_17, day_18, day_19, day_20, day_21, day_22 };

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

    if (day < 1 || day > 25) {
        std::cout << "Invalid day. Day must be a number between 1 and 25"
                  << std::endl;
        return 1;
    }

    if (static_cast<int>(sizeof(days) / sizeof(*days)) >= day) {
        days[day - 1](argv[2]);
    } else {
        std::cout << "Day not implemented" << std::endl;
        return 1;
    }

    return 0;
}
