#include "FigureCounter.h"

#include <queue>

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

        // Visits all cells of one figure, starting from (row, col).
        // A visited cell is unmarked, so it is never visited twice.
        // The queue is made once by the caller and is empty again at the end.
        void visitFigure(Grid& grid, std::queue<Cell>& pending, int row, int col)
        {
            grid.unmark(row, col);

            pending.push(Cell{row, col});

            while (!pending.empty())
            {
                const Cell cell = pending.front();
                pending.pop();

                for (const Offset& offset : kNeighborOffsets)
                {
                    const int neighborRow = cell.row + offset.row;
                    const int neighborCol = cell.col + offset.col;

                    if (grid.isMarked(neighborRow, neighborCol))
                    {
                        grid.unmark(neighborRow, neighborCol);

                        pending.push(Cell{neighborRow, neighborCol});
                    }
                }
            }
        }
    } // namespace

    std::size_t countFigures(Grid grid)
    {
        std::size_t count = 0;

        // One queue for all figures. Making a new one for every figure is slow
        // when there are millions of small figures.
        std::queue<Cell> pending;

        for (int row = 0; row < grid.rows(); row++)
        {
            for (int col = 0; col < grid.cols(); col++)
            {
                // A marked cell here is a new figure.
                // The cells of the figures counted so far are already unmarked.
                if (grid.isMarked(row, col))
                {
                    visitFigure(grid, pending, row, col);
                    count++;
                }
            }
        }

        return count;
    }

} // namespace figures
