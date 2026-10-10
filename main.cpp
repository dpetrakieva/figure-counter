#include "FigureCounter.h"
#include "GridReader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>

using namespace figures;

// Reads a grid from a text file and prints the number of figures in it.
// Usage: figure_counter_app <input file>
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: figure_counter_app <input file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    try
    {
        const std::size_t count = countFigures(readGrid(file));

        std::cout << count << (count == 1 ? " figure" : " figures") << "\n";
    }
    catch (const std::invalid_argument& error)
    {
        std::cerr << "Invalid input: " << error.what() << "\n";
        return 1;
    }
    catch (const std::bad_alloc&)
    {
        std::cerr << "Not enough memory for this grid.\n";
        return 1;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
