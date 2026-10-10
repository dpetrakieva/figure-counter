#include "Grid.h"

#include <stdexcept>
#include <utility>

namespace figures
{
    namespace
    {
        // How many cells the grid has.
        // The cast is needed because rows * cols can be too big for an int.
        std::size_t cellCount(int rows, int cols)
        {
            if (rows < 0 || cols < 0)
            {
                throw std::invalid_argument("The number of rows and columns cannot be negative.");
            }

            return static_cast<std::size_t>(rows) * cols;
        }
    } // namespace

    Grid::Grid(int rows, int cols)
        : m_rows(rows),
          m_cols(cols),
          m_cells(cellCount(rows, cols), false)
    {
    }

    Grid::Grid(int rows, int cols, const std::vector<bool>& cells)
        : m_rows(rows),
          m_cols(cols),
          m_cells(cells)
    {
        checkCellCount();
    }

    Grid::Grid(int rows, int cols, std::vector<bool>&& cells)
        : m_rows(rows),
          m_cols(cols),
          m_cells(std::move(cells))
    {
        checkCellCount();
    }

    void Grid::checkCellCount() const
    {
        if (m_cells.size() != cellCount(m_rows, m_cols))
        {
            throw std::invalid_argument("The number of cells must be rows * cols.");
        }
    }

} // namespace figures
