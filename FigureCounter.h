#pragma once

#include <cstddef>
#include <vector>

namespace figures
{
    // The matrix. Each inner vector is one row. true = marked cell.
    using Grid = std::vector<std::vector<bool>>;

    // Counts the figures in the grid.
    // Cells that touch by a side are one figure. Diagonal does not count.
    //
    // The grid is taken by value, because the function unmarks the cells it has visited.
    // The grid of the caller is not changed.
    //
    // Throws std::invalid_argument if the rows have different lengths.
    // Time: O(rows * cols).
    // Memory: a stack of pending cells.
    std::size_t countFigures(Grid grid);

} // namespace figures
