#include "FigureCounter.h"

#include <queue>
#include <stdexcept>

namespace figures
{
    namespace
    {
        // One step to a neighbor cell.
        struct Offset
        {
            int row;
            int col;
        };

        // A cell position in the grid.
        struct Cell
        {
            int row;
            int col;
        };

        // Neighbors: up, down, left, right. No diagonals.
        constexpr Offset kNeighborOffsets[] =
        {
            {-1,  0},      // up
            { 1,  0},      // down
            { 0, -1},      // left
            { 0,  1}       // right
        };

        // True if all rows have the same length.
        bool isRectangular(const Grid& grid)
        {
            if (grid.empty())
            {
                return true;
            }

            const std::size_t cols = grid[0].size();

            for (const std::vector<bool>& row : grid)
            {
                if (row.size() != cols)
                {
                    return false;
                }
            }

            return true;
        }

        // True if (row, col) is inside the grid.
        bool isInside(int row, int col, int rows, int cols)
        {
            return (row >= 0 &&
                    row < rows &&
                    col >= 0 &&
                    col < cols);
        }

        // Visits all cells of one figure, starting from (row, col).
        // A visited cell is unmarked, so it is never visited twice.
        void visitFigure(Grid& grid, int row, int col)
        {
            const int rows = static_cast<int>(grid.size());
            const int cols = static_cast<int>(grid[0].size());

            grid[row][col] = false;

            // Cells that are found but not processed yet.
            std::queue<Cell> pending;
            pending.push(Cell{row, col});

            while (!pending.empty())
            {
                const Cell cell = pending.front();
                pending.pop();

                for (const Offset& offset : kNeighborOffsets)
                {
                    const int neighborRow = cell.row + offset.row;
                    const int neighborCol = cell.col + offset.col;

                    if (isInside(neighborRow, neighborCol, rows, cols) && grid[neighborRow][neighborCol])
                    {
                        grid[neighborRow][neighborCol] = false;

                        pending.push(Cell{neighborRow, neighborCol});
                    }
                }
            }
        }
    } // namespace

    std::size_t countFigures(Grid grid)
    {
        if (!isRectangular(grid))
        {
            throw std::invalid_argument("All rows must have the same length.");
        }

        if (grid.empty() || grid[0].empty())
        {
            return 0;
        }

        std::size_t count = 0;

        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                // A marked cell here is a new figure.
                // The cells of the figures counted so far are already unmarked.
                if (grid[row][col])
                {
                    visitFigure(grid, row, col);
                    count++;
                }
            }
        }

        return count;
    }

} // namespace figures
