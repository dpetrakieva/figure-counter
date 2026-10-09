#include "FigureCounter.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace figures;

// One test: a name, a grid, and the expected number of figures.
struct TestCase
{
    std::string name;
    Grid grid;
    std::size_t expected;
};

// One invalid input: a name and a grid that must be rejected.
struct InvalidCase
{
    std::string name;
    Grid grid;
};

// Makes a grid where every cell is marked.
Grid makeFullGrid(int rows, int cols)
{
    return Grid(rows, std::vector<bool>(cols, true));
}

// Makes a grid like a chessboard. Marked cells touch only diagonally.
Grid makeCheckerboardGrid(int rows, int cols)
{
    Grid grid(rows, std::vector<bool>(cols, false));

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if ((row + col) % 2 == 0)
            {
                grid[row][col] = true;
            }
        }
    }

    return grid;
}

// Checks the number of figures for each grid. Returns the number of failed tests.
int runCountTests()
{
    const std::vector<TestCase> testCases =
    {
        { "task example",                   { {1, 0, 0, 0, 0},
                                              {1, 0, 1, 1, 0},
                                              {0, 1, 0, 1, 0},
                                              {0, 1, 1, 1, 0},
                                              {0, 0, 0, 0, 1} }, 3 },
        { "empty grid",                     { },                 0 },
        { "nothing marked",                 { {0, 0, 0},
                                              {0, 0, 0} },       0 },
        { "everything marked",              { {1, 1, 1},
                                              {1, 1, 1} },       1 },
        { "single marked cell",             { {1} },             1 },
        { "single unmarked cell",           { {0} },             0 },
        { "two cells touching by a vertex", { {1, 0},
                                              {0, 1} },          2 },
        { "checkerboard",                   { {1, 0, 1},
                                              {0, 1, 0},
                                              {1, 0, 1} },       5 },
        { "single row",                     { {1, 1, 0, 1} },    2 },
        { "single column",                  { {1},
                                              {0},
                                              {1},
                                              {1} },             2 },
        { "U shape is one figure",          { {1, 0, 1},
                                              {1, 0, 1},
                                              {1, 1, 1} },       1 },
        { "rows with no cells",             { {},
                                              {} },              0 },
        { "ring with a hole is one figure", { {1, 1, 1},
                                              {1, 0, 1},
                                              {1, 1, 1} },       1 },
        { "figure inside a ring",           { {1, 1, 1, 1, 1},
                                              {1, 0, 0, 0, 1},
                                              {1, 0, 1, 0, 1},
                                              {1, 0, 0, 0, 1},
                                              {1, 1, 1, 1, 1} }, 2 },
        { "wide grid, two separate cells",  { {0, 0, 1, 0, 0},
                                              {1, 0, 0, 0, 0} }, 2 },
        { "tall grid, two separate cells",  { {0, 1},
                                              {0, 0},
                                              {1, 0},
                                              {0, 0},
                                              {0, 0} },          2 },

        // Large grids: 25 million cells each, in different shapes.
        { "large, everything marked",       makeFullGrid(5000, 5000),         1 },
        { "large, checkerboard",            makeCheckerboardGrid(5000, 5000), 5000 * 5000 / 2 },
        { "large, one column",              makeFullGrid(25000000, 1),        1 },
        { "large, one row",                 makeFullGrid(1, 25000000),        1 },
    };

    int failedCount = 0;

    for (const TestCase& testCase : testCases)
    {
        const std::size_t actual = countFigures(testCase.grid);

        if (actual == testCase.expected)
        {
            std::cout << "PASS: " << testCase.name << std::endl;
        }
        else
        {
            std::cout << "FAIL: " << testCase.name
                      << " (expected " << testCase.expected
                      << ", got " << actual << ")" << std::endl;

            failedCount++;
        }
    }

    return failedCount;
}

// Checks that invalid input is rejected. Returns the number of failed tests.
int runInvalidInputTests()
{
    const std::vector<InvalidCase> testCases =
    {
        { "second row is shorter", { {1, 0, 1},
                                     {1, 0} } },
        { "second row is longer",  { {1, 0},
                                     {1, 0, 1} } },
        { "first row is empty",    { {},
                                     {1} } },
    };

    int failedCount = 0;

    for (const InvalidCase& testCase : testCases)
    {
        try
        {
            countFigures(testCase.grid);

            std::cout << "FAIL: " << testCase.name << " (no exception)" << std::endl;
            failedCount++;
        }
        catch (const std::invalid_argument&)
        {
            std::cout << "PASS: " << testCase.name << std::endl;
        }
    }

    return failedCount;
}

int main()
{
    const int failedCount = runCountTests() + runInvalidInputTests();

    return (failedCount == 0) ? 0 : 1;
}
