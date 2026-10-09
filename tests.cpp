#include "FigureCounter.h"
#include "Grid.h"

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

// Makes a grid from rows written one under another. 1 = marked cell.
// All rows must have the same length.
Grid makeGrid(const std::vector<std::vector<bool>>& rows)
{
    if (rows.empty())
    {
        return Grid(0, 0);
    }

    std::vector<bool> cells;

    for (const std::vector<bool>& row : rows)
    {
        for (bool cell : row)
        {
            cells.push_back(cell);
        }
    }

    return Grid(static_cast<int>(rows.size()), static_cast<int>(rows[0].size()), cells);
}

// Makes a grid where every cell is marked.
Grid makeFullGrid(int rows, int cols)
{
    Grid grid(rows, cols);

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            grid.mark(row, col);
        }
    }

    return grid;
}

// Makes a grid like a chessboard. Marked cells touch only diagonally.
Grid makeCheckerboardGrid(int rows, int cols)
{
    Grid grid(rows, cols);

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if ((row + col) % 2 == 0)
            {
                grid.mark(row, col);
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
        { "task example",                   makeGrid({ {1, 0, 0, 0, 0},
                                                       {1, 0, 1, 1, 0},
                                                       {0, 1, 0, 1, 0},
                                                       {0, 1, 1, 1, 0},
                                                       {0, 0, 0, 0, 1} }), 3 },
        { "empty grid",                     makeGrid({ }),                 0 },
        { "nothing marked",                 makeGrid({ {0, 0, 0},
                                                       {0, 0, 0} }),       0 },
        { "everything marked",              makeGrid({ {1, 1, 1},
                                                       {1, 1, 1} }),       1 },
        { "single marked cell",             makeGrid({ {1} }),             1 },
        { "single unmarked cell",           makeGrid({ {0} }),             0 },
        { "two cells touching by a vertex", makeGrid({ {1, 0},
                                                       {0, 1} }),          2 },
        { "checkerboard",                   makeGrid({ {1, 0, 1},
                                                       {0, 1, 0},
                                                       {1, 0, 1} }),       5 },
        { "single row",                     makeGrid({ {1, 1, 0, 1} }),    2 },
        { "single column",                  makeGrid({ {1},
                                                       {0},
                                                       {1},
                                                       {1} }),             2 },
        { "U shape is one figure",          makeGrid({ {1, 0, 1},
                                                       {1, 0, 1},
                                                       {1, 1, 1} }),       1 },
        { "rows with no cells",             makeGrid({ {},
                                                       {} }),              0 },
        { "ring with a hole is one figure", makeGrid({ {1, 1, 1},
                                                       {1, 0, 1},
                                                       {1, 1, 1} }),       1 },
        { "figure inside a ring",           makeGrid({ {1, 1, 1, 1, 1},
                                                       {1, 0, 0, 0, 1},
                                                       {1, 0, 1, 0, 1},
                                                       {1, 0, 0, 0, 1},
                                                       {1, 1, 1, 1, 1} }), 2 },
        { "wide grid, two separate cells",  makeGrid({ {0, 0, 1, 0, 0},
                                                       {1, 0, 0, 0, 0} }), 2 },
        { "tall grid, two separate cells",  makeGrid({ {0, 1},
                                                       {0, 0},
                                                       {1, 0},
                                                       {0, 0},
                                                       {0, 0} }),          2 },

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

// Checks what a grid does with wrong arguments.
// Returns the number of failed tests.
int runGridTests()
{
    int failedCount = 0;

    // A grid of 2 rows and 3 columns needs 6 cells. Here it gets only 5.
    const std::vector<bool> fiveCells = {1, 0, 1, 0, 1};

    try
    {
        Grid(2, 3, fiveCells);

        std::cout << "FAIL: wrong number of cells (no exception)" << std::endl;
        failedCount++;
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "PASS: wrong number of cells" << std::endl;
    }

    // Every cell is marked, so "not marked" can only come from the position check.
    const Grid grid = makeFullGrid(2, 3);

    // A cell outside the grid is not marked.
    if (!grid.isMarked(-1, 0) && !grid.isMarked(2, 0) && !grid.isMarked(1, -1) && !grid.isMarked(0, 3))
    {
        std::cout << "PASS: cell outside the grid is not marked" << std::endl;
    }
    else
    {
        std::cout << "FAIL: cell outside the grid is not marked" << std::endl;
        failedCount++;
    }

    // A cell outside the grid cannot be marked.
    try
    {
        Grid emptyGrid(2, 3);
        emptyGrid.mark(2, 0);

        std::cout << "FAIL: marking outside the grid (no exception)" << std::endl;
        failedCount++;
    }
    catch (const std::out_of_range&)
    {
        std::cout << "PASS: marking outside the grid" << std::endl;
    }

    return failedCount;
}

int main()
{
    const int failedCount = runCountTests() + runGridTests();

    return (failedCount == 0) ? 0 : 1;
}
