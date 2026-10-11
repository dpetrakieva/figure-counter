# Figure Counter

Counts the figures in a matrix of marked and unmarked cells.

A figure is a group of marked cells that touch each other by a side.
Cells that touch only by a corner (diagonally) belong to different figures.

The example from the task, where `*` is a marked cell:

```
*....
*.**.
.*.*.
.***.
....*
```

The result is `3 figures`.

## Build

You need CMake 3.20 or newer and a C++17 compiler.

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run the tests:

```
ctest --test-dir build -C Release --output-on-failure
```

The project is built and tested on Windows (MSVC), Linux (GCC) and macOS (Clang)
on every push, with GitHub Actions. Compiler warnings are turned on: `/W4` on MSVC,
`-Wall -Wextra` on GCC and Clang.

## Usage

```
figure_counter_app <input file>
```

The program reads the grid from the file and prints the number of figures.
The format of the file is described in the next section.

For example, `example.txt` holds the example above:

```
./build/figure_counter_app example.txt               (Linux, macOS)
build\Release\figure_counter_app.exe example.txt     (Windows, Visual Studio generator)
```

It prints `3 figures`.

The program exits with code 0 on success and with code 1 if something is wrong.

## Input

The input is a text file. Each line is one row of the matrix.
`*` is a marked cell and `.` is a cell that is not marked.

Lines can end the Windows way (`\r\n`) or the Unix way (`\n`).
The last line does not need a line ending.
An empty line is a row with no cells, so an empty line after the last row is an error.

`example_16_figures.txt` is a larger example:

```
..............................
..****......*.........***.....
..*..*.....***........*.*.....
..*..*......*.........***.....
..****...........**...........
.................**......*....
....*...................*.*...
...*.*.......*****.......*....
....*........*................
.............*....**..........
..***........*....**....****..
..............................
```

It has 16 figures. The two diamonds are 4 figures each, because their cells
touch only by a corner.

If something is wrong, the program prints one of these messages:

| Problem | Message |
| --- | --- |
| No file name, or more than one | `Usage: figure_counter_app <input file>` |
| The file cannot be opened | `Cannot open file: <name>` |
| The file cannot be read | `Error: Cannot read the input.` |
| A character other than `*` or `.` | `Invalid input: Unexpected character at row <r>, column <c>.` |
| Rows with different lengths | `Invalid input: Row <r> has a different length than the first row.` |
| Not enough memory for the grid | `Not enough memory for this grid.` |
| Any other error | `Error: <message>` |

An empty file is valid and has 0 figures.

## Project structure

| File | What it is |
| --- | --- |
| `Grid.h`, `Grid.cpp` | The grid of cells. `Grid`. |
| `FigureCounter.h`, `FigureCounter.cpp` | Counts the figures. `countFigures`. |
| `GridReader.h`, `GridReader.cpp` | Reads a grid from text. `readGrid`. |
| `main.cpp` | The program: reads a file and prints the number of figures. |
| `tests.cpp` | The tests. A separate program, run by CTest. |
| `example.txt` | The example from the task. |
| `example_16_figures.txt` | A larger example with different shapes. |
| `tree_21x21.txt` | A small figure shaped like a tree. See "Large matrices". |
| `CMakeLists.txt` | The build. |
| `.github/workflows/ci.yml` | Builds and tests on Windows, Linux and macOS. |

`Grid`, `FigureCounter` and `GridReader` are built as one static library.
The program and the tests both use it.

## How it works

1. Go through the grid cell by cell.
2. When a marked cell is found, a new figure starts. Add one to the count.
3. Visit the whole figure: start from that cell and keep going to marked neighbors
   (up, down, left, right) until there are no new ones. Every visited cell is unmarked,
   so it is not visited again.
4. Continue from step 1. Figures that were already counted have no marked cells left,
   so they are skipped.

The cells that are found but not processed yet wait in a queue.

`countFigures` works on its own grid, so the grid of the caller is not changed.

## Complexity

- Time: `O(rows * cols)`. Every cell is looked at a fixed number of times.
- Memory: one bit per cell for the grid, plus the queue of waiting cells.
  There is no second grid for the visited cells.

## Large matrices

The task asks for a solution that works with millions of cells.
The tests include four grids of 25 million cells each: two of 5000 x 5000,
one with a single column and one with a single row.

The solution got there in five steps. Each step is one commit in the git history.

1. **Recursion.** Simple, but each cell of a figure adds one call to the call stack.
   A large figure causes a stack overflow.
2. **A stack on the heap instead of recursion.** No more stack overflow, and the large
   grids can be tested. On a 5000 x 5000 grid with every cell marked, the stack held
   up to 12,497,501 waiting cells.
3. **A queue instead of a stack.** On the same grid the queue holds up to 5,000 waiting cells.
4. **One flat vector instead of a vector of rows.** In a vector of rows, every row is
   a separate vector with its own block of memory. That costs a lot when there are many
   short rows, as in the grid with 25,000,000 rows and 1 column.
5. **One queue for all figures.** Making a new queue for every figure is slow
   when the grid has millions of small figures.

What the whole test program needed after each step:

| Step | Time | Memory | Most waiting cells |
| --- | --- | --- | --- |
| 2. Stack | about 7.2 s | about 2.6 GB | 12,497,501 |
| 3. Queue | about 6.9 s | about 2.6 GB | 5,000 |
| 4. Flat vector | about 2.5 s | about 17 MB | 5,000 |
| 5. One queue for all figures | about 1.3 s | about 17 MB | 5,000 |

Step 1 is not in the table: with recursion the large grids cannot run at all.

How it was measured: Visual Studio, x64 Release build. The time is from `Measure-Command`
in PowerShell, repeated several times. The memory is the peak of Process Memory in
Diagnostic Tools.

### Queue or stack

A stack goes deep into the figure and leaves waiting cells behind along the whole path.
A queue visits the figure in waves, so it holds only the current front of the wave.

Which one needs less memory depends on the figure.

- **A filled figure.** The queue holds about one line of cells across the figure.
  The stack can hold cells from the whole area. On the 5000 x 5000 grid with every cell
  marked, that is 5,000 waiting cells in the queue against 12,497,501 in the stack.
- **A thin figure with many branches.** If all the branches end at the same distance
  from the start, all the branch ends wait in the queue at the same time.
  `tree_21x21.txt` is a small figure of this kind. The same tree in a 5000 x 5000 grid
  is one figure of about 7 million cells. On it the queue held up to 1,572,864 waiting
  cells and the program needed about 52 MB.

This tree is not the worst figure for the queue. The worst one is not known.
There is a limit for both the queue and the stack: the waiting cells are never more than
about three quarters of the cells of the figure. Every waiting cell was added by a cell
that was already taken out, and one cell adds at most three.

The queue was chosen for the filled figures. There the difference is very large,
and the figure does not need a special shape.

The tree was measured with the program itself and a temporary counter.
Its input file is 25 MB, so it is not in the repository.

## Decisions

- **A cell is one bit.** A cell is either marked or not. The cells are kept in
  a `std::vector<bool>`, which uses one bit per cell.
- **The grid is a small class.** `Grid` keeps the cells in one vector, together with the
  number of rows and columns. The constructor checks that they match, so a grid with rows
  of different lengths cannot exist and `countFigures` does not need to check its input.
- **`Grid` checks the position.** `isMarked` answers "no" for a cell outside the grid.
  `mark` and `unmark` throw `std::out_of_range`. The class cannot read or write outside
  its vector.
- **The small functions of `Grid` are in the header.** They are called for every cell.
  In the header the compiler can inline them. The constructors stay in `Grid.cpp`.
- **`countFigures` takes the grid by value.** It unmarks the cells it has visited, so it
  needs a grid that it can change. Taking it by value keeps the grid of the caller
  unchanged. The program passes the grid straight from `readGrid`, so there it is not copied.
- **Two ways to make a grid from a list of cells.** One constructor copies the list.
  The other takes it over without a copy. `readGrid` uses the second one.
- **`int` for row and column, `std::size_t` for counts and indexes.** Row and column can
  become `-1` when looking at a neighbor, so they are signed. The number of cells and the
  number of figures can be larger than an `int`, so they are `std::size_t`.
  This limits each side of the grid to the largest `int`.
- **Plain text input with `*` and `.`.** The figures are easy to see, also in a larger
  grid. The file is easy to write by hand and needs no extra library to read.
  The two characters are named constants in `GridReader.cpp`.
- **The reader takes a stream, not a file name.** The same function reads from a file
  in the program and from a string in the tests.
- **Invalid input throws.** `readGrid` and the `Grid` constructors throw
  `std::invalid_argument`. The program catches it, prints the message and exits with code 1.
  The message says the row, so the problem is easy to find in a long file.
  If the input cannot be read at all, `readGrid` throws `std::runtime_error`,
  so a read error is never taken for the end of the file.
- **Counting, reading and the grid are tested separately.** The counting tests pass
  a grid directly. The reading tests pass text, and go through both reading and counting.
  The tests for invalid input check the exact error message, so a wrong row number is
  caught. The grid has its own tests for positions outside of it.
- **No test framework.** The tests are tables of inputs and expected results in a separate
  program. CTest runs it and checks the exit code. This way the project needs nothing
  but CMake and a compiler: there is no library to download or install, and it builds
  the same way on Windows, Linux and macOS.
- **A static library.** The library is small and is used only inside this project.
  The program stays one file that runs by itself. A shared library would need
  export macros on Windows and one more file to ship, with no benefit here.

## Possible improvements

These are left out to keep the solution simple, as the task asks.

- **Diagonal neighbors.** The neighbors are listed in one array, so counting diagonal
  contact as well needs only four more lines there.
- **Grids that do not fit in memory.** The file could be processed row by row, keeping only
  the previous row and joining figures with a union-find structure.
- **Parallel processing.** The grid could be split into bands processed in parallel,
  with the figures joined at the band borders.
