#ifndef GRID_H
#define GRID_H

#define CELL_EMPTY   '.'
#define CELL_PERSON  'S'
#define CELL_EXIT    'E'
#define CELL_OBSTACLE 'X'
#define CELL_FIRE    'F'

#define MAX_GRID_ROWS 50
#define MAX_GRID_COLS 50
#define MAX_PATH_LEN  (MAX_GRID_ROWS * MAX_GRID_COLS)

typedef struct {
    int row;
    int col;
} Node;

typedef struct {
    char cells[MAX_GRID_ROWS][MAX_GRID_COLS];
    int rows;
    int cols;
    Node start;
    Node exit;
    int has_start;
    int has_exit;
} Grid;

int grid_init(Grid *grid);
int grid_parse_json(const char *json, Grid *grid);
int grid_is_walkable(const Grid *grid, int row, int col);
int grid_is_exit(const Grid *grid, int row, int col);
int grid_in_bounds(const Grid *grid, int row, int col);
void grid_free(Grid *grid);

#endif
