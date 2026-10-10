#pragma once

#include "Grid.h"

#include <istream>

namespace figures
{
    // Reads a grid from text. Each line is one row: '*' is a marked cell, '.' is not.
    // Throws std::invalid_argument if there is any other character,
    // or if the lines have different lengths.
    // Throws std::runtime_error if the input cannot be read.
    Grid readGrid(std::istream& input);

} // namespace figures
