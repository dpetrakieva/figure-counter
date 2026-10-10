#include "GridReader.h"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace figures
{
    namespace
    {
        // The characters of the input text.
        constexpr char kMarkedCell = '*';
        constexpr char kUnmarkedCell = '.';
    } // namespace

    Grid readGrid(std::istream& input)
    {
        std::vector<bool> cells;
        int rows = 0;
        int cols = 0;

        std::string line;

        while (std::getline(input, line))
        {
            // A file saved on Windows ends each line with "\r\n". Remove the '\r'.
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            // Row and column numbers in the messages start from 1.
            const int rowNumber = rows + 1;

            const int length = static_cast<int>(line.size());

            // The first line decides how many columns the grid has.
            if (rows == 0)
            {
                cols = length;
            }
            else if (length != cols)
            {
                throw std::invalid_argument("Row " + std::to_string(rowNumber) +
                                            " has a different length than the first row.");
            }

            for (int col = 0; col < length; col++)
            {
                if (line[col] == kMarkedCell)
                {
                    cells.push_back(true);
                }
                else if (line[col] == kUnmarkedCell)
                {
                    cells.push_back(false);
                }
                else
                {
                    throw std::invalid_argument("Unexpected character at row " + std::to_string(rowNumber) +
                                                ", column " + std::to_string(col + 1) + ".");
                }
            }

            rows++;
        }

        // The loop also stops if the input cannot be read, for example on a disk error.
        if (input.bad())
        {
            throw std::runtime_error("Cannot read the input.");
        }

        return Grid(rows, cols, std::move(cells));
    }

} // namespace figures
