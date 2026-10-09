#pragma once

#include "Grid.h"

#include <cstddef>

namespace figures
{
    // Counts the figures in the grid.
    // Cells that touch by a side are one figure. Diagonal does not count.
    //
    // The grid is taken by value, because the function unmarks the cells it has visited.
    // The grid of the caller is not changed.
    //
    // Time: O(rows * cols).
    // Memory: a queue of pending cells.
    std::size_t countFigures(Grid grid);

} // namespace figures
