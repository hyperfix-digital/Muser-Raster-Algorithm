# Muser Raster Algorithm
Low fidelity data rasterization algorithm to be used in [Muser](https://github.com/anthonymesa/Muser).

## About
Given a 20x20 grid, if we break the grid into 6 triangular faces given only points composed of x, y, and z values where z connotates a magnitude we can interpolate the data between those 6 points in O(n) time (where n = x_grid_dimension * y_grid_dimension) using a rasterisation algorithm.

## Example

### Points

point 1 -> (0, 0, 9)\
point 2 -> (5, 15, 1)\
point 3 -> (0, 19, 6)\
point 4 -> (14, 3, 5)\
point 5 -> (19, 0, 3)\
point 6 -> (19, 19, 2)

### Faces

face 1 -> (0, 0, 9)  (5, 15, 1) (0, 19, 6)\
face 2 -> (0, 0, 9)  (5, 15, 1) (14, 3, 5)\
face 3 -> (0, 0, 9)  (14, 3, 5) (19, 0, 3)\
face 4 -> (5, 15, 1) (14, 3, 5) (19, 19, 2)\
face 5 -> (19, 0, 3) (14, 3, 5) (19, 19, 2)\
face 6 -> (0, 19, 6) (5, 15, 1) (19, 19, 2)

### Interpolation result
9 8 8 8 7 7 7 6 6 6 5 5 5 4 4 4 3 3 3 3 \
8 8 8 7 7 7 7 6 6 6 5 5 5 5 4 4 4 3 3 2 \
7 7 7 7 7 7 6 6 6 6 6 5 5 5 4 4 4 3 3 2 \
8 7 7 7 6 6 6 6 6 5 5 5 5 5 5 4 4 3 3 2 \
8 6 6 6 6 6 6 5 5 5 5 5 4 4 4 4 4 3 3 2 \
8 6 6 6 5 5 5 5 5 4 4 4 4 4 4 4 4 3 3 2 \
8 6 5 5 5 5 5 4 4 4 4 4 4 4 4 4 3 3 3 2 \
7 6 5 5 4 4 4 4 4 4 3 3 3 3 4 4 3 3 3 2 \
7 6 4 4 4 4 4 3 3 3 3 3 3 3 3 3 3 3 3 2 \
7 6 5 4 4 3 3 3 3 3 3 3 3 3 3 3 3 3 2 2 \
7 6 5 3 3 3 3 2 2 2 2 2 3 3 3 3 3 3 2 2 \
7 6 5 3 3 2 2 2 2 2 2 2 2 3 3 3 3 3 2 2 \
7 5 4 3 2 2 2 2 2 2 2 2 2 2 2 3 2 3 2 2 \
6 5 4 3 2 1 1 1 1 2 2 2 2 2 2 2 2 3 2 2 \
6 5 4 3 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 \
6 5 4 3 2 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 \
6 5 4 2 2 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 \
6 5 3 3 3 2 2 2 2 2 1 1 1 1 1 1 2 2 2 2 \
6 4 4 4 4 3 3 3 3 3 2 2 2 2 2 1 1 1 1 2 \
6 5 5 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 2 

Further write-up pending...
