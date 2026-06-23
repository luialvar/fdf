# FdF

Wireframe 3D map renderer developed in C as part of the 42 curriculum.

## About

FdF is a graphics project that reads a map from a `.fdf` file and renders it as a 3D wireframe projection.

Each number in the input file represents a height value. The program parses the map, stores it in a matrix and draws the corresponding 3D terrain using MLX42.

This project focuses on file parsing, matrix handling, basic computer graphics, projections and low-level window rendering in C.

## Features

- Reads `.fdf` map files
- Parses height values into a matrix
- Draws a wireframe representation of the map
- Uses MLX42 for window and pixel rendering
- Handles dynamic map sizes
- Basic 3D-to-2D projection
- Makefile-based compilation

## Project Structure

```txt
.
├── Makefile
├── main.c
├── create_matrix.c
├── printing.c
├── external_functions.c
├── ft_split.c
├── fdf.h
├── get_next_line/
├── src/
├── pruebas/
└── libmlx42.a
Compilation
make

This creates the executable:

./fdf

To clean object files:

make clean

To remove object files and the executable:

make fclean

To rebuild:

make re
Usage
./fdf maps/map.fdf

Example .fdf map:

0 0 0 0
0 1 2 0
0 2 1 0
0 0 0 0

Each value represents the height of a point in the map.

Dependencies
C compiler
Make
MLX42
GLFW
OpenGL
math library

On Linux, you may need:

sudo apt install libglfw3-dev
What I Learned

Through this project I practiced:

Reading and parsing files in C
Using get_next_line
Working with dynamic matrices
Drawing pixels and lines manually
Understanding basic 3D projection
Managing graphical windows with MLX42
Structuring a graphical C project
Writing a Makefile with external libraries
Technologies
C
MLX42
GLFW
OpenGL
Makefile
Notes

This project is part of the 42 curriculum.
It is mainly focused on learning how graphical rendering works at a low level, without using high-level game engines or graphics frameworks.

If you are also a 42 student, use this repository only as a reference. The best way to learn FdF is to understand the projection, parsing and drawing logic by yourself.

Author

Luis Ángel Álvarez Gil
42 Málaga
