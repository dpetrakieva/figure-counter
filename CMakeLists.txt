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

add_executable(figure_counter main.cpp)
