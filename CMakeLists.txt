cmake_minimum_required(VERSION 3.20)
project(FigureCounter CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Show more warnings.
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# The logic, as a library used by both programs below.
add_library(figure_counter FigureCounter.cpp Grid.cpp)

# The program: prints the result for the example from the task.
add_executable(figure_counter_app main.cpp)
target_link_libraries(figure_counter_app PRIVATE figure_counter)

# The tests: a separate program, run with ctest.
enable_testing()
add_executable(figure_counter_tests tests.cpp)
target_link_libraries(figure_counter_tests PRIVATE figure_counter)
add_test(NAME figure_counter_tests COMMAND figure_counter_tests)
