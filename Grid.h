#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace figures
{
    // A grid of cells. A cell is marked or not marked.
    // All cells are in one vector: first row 0, then row 1, and so on.
    class Grid
    {
    public:
        // Makes a grid with no marked cells.
        // Throws std::invalid_argument if rows or cols is negative.
        Grid(int rows, int cols);

        // Makes a grid from a list of cells. true = marked.
        // The first one copies the list. The second one takes it over, without a copy.
        // Throws std::invalid_argument if the list does not have rows * cols cells.
        Grid(int rows, int cols, const std::vector<bool>& cells);
        Grid(int rows, int cols, std::vector<bool>&& cells);

        // The functions below are called for every cell.
        // They are defined here, in the header, so the compiler can inline them.
        int rows() const
        {
            return m_rows;
        }

        int cols() const
        {
            return m_cols;
        }

        // Is the cell marked? A cell outside the grid is not marked.
        bool isMarked(int row, int col) const
        {
            return contains(row, col) && m_cells[index(row, col)];
        }

        // Marks the cell.
        // Throws std::out_of_range if (row, col) is outside the grid.
        void mark(int row, int col)
        {
            set(row, col, true);
        }

        // Removes the mark from the cell.
        // Throws std::out_of_range if (row, col) is outside the grid.
        void unmark(int row, int col)
        {
            set(row, col, false);
        }

    private:
        // Throws std::invalid_argument if the number of cells is not rows * cols.
        void checkCellCount() const;

        // Is (row, col) inside the grid?
        bool contains(int row, int col) const
        {
            return (row >= 0 &&
                    row < m_rows &&
                    col >= 0 &&
                    col < m_cols);
        }

        // Where the cell (row, col) is in the vector.
        std::size_t index(int row, int col) const
        {
            return static_cast<std::size_t>(row) * m_cols + col;
        }

        // Marks the cell or removes its mark.
        void set(int row, int col, bool marked)
        {
            if (!contains(row, col))
            {
                throw std::out_of_range("The cell is outside the grid.");
            }

            m_cells[index(row, col)] = marked;
        }

        int m_rows;
        int m_cols;
        std::vector<bool> m_cells;
    };

} // namespace figures
