#include "FigureCounter.h"

#include <iostream>

using namespace figures;

int main()
{
    // The example from the task statement; expected result: 3 figures.
    const Grid grid =
    {
        {1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 1}
    };

    const std::size_t count = countFigures(grid);

    std::cout << count << (count == 1 ? " figure" : " figures") << "\n";

    return 0;
}
