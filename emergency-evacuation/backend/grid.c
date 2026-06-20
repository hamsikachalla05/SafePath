#include "grid.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int grid_init(Grid *grid) {
    if (!grid) {
        return -1;
    }

    memset(grid, 0, sizeof(Grid));
    grid->rows = 0;
    grid->cols = 0;
    grid->has_start = 0;
    grid->has_exit = 0;
    return 0;
}

static void grid_register_cell(Grid *grid, int row, int col, char cell) {
    grid->cells[row][col] = cell;

    if (cell == CELL_PERSON) {
        grid->start.row = row;
        grid->start.col = col;
        grid->has_start = 1;
    } else if (cell == CELL_EXIT) {
        grid->exit.row = row;
        grid->exit.col = col;
        grid->has_exit = 1;
    }
}

static const char *skip_whitespace(const char *text) {
    while (*text && isspace((unsigned char)*text)) {
        text++;
    }
    return text;
}

static int parse_grid_array(const char *json, Grid *grid) {
    const char *cursor = strstr(json, "\"grid\"");
    if (!cursor) {
        return -1;
    }

    cursor = strchr(cursor, '[');
    if (!cursor) {
        return -1;
    }

    cursor++;
    grid->rows = 0;
    grid->cols = 0;
    grid->has_start = 0;
    grid->has_exit = 0;

    while (*cursor) {
        cursor = skip_whitespace(cursor);
        if (*cursor == ']') {
            break;
        }

        if (*cursor != '[') {
            return -1;
        }

        cursor++;
        int col = 0;

        while (*cursor) {
            cursor = skip_whitespace(cursor);
            if (*cursor == ']') {
                cursor++;
                break;
            }

            if (*cursor != '"') {
                return -1;
            }

            cursor++;
            if (!*cursor || *cursor == '"') {
                return -1;
            }

            char cell = *cursor;
            cursor++;

            if (*cursor != '"') {
                return -1;
            }
            cursor++;

            if (grid->rows >= MAX_GRID_ROWS || col >= MAX_GRID_COLS) {
                return -1;
            }

            grid_register_cell(grid, grid->rows, col, cell);
            col++;

            cursor = skip_whitespace(cursor);
            if (*cursor == ',') {
                cursor++;
            }
        }

        if (grid->cols == 0) {
            grid->cols = col;
        } else if (grid->cols != col) {
            return -1;
        }

        grid->rows++;
        cursor = skip_whitespace(cursor);
        if (*cursor == ',') {
            cursor++;
        }
    }

    if (grid->rows == 0 || grid->cols == 0) {
        return -1;
    }

    return 0;
}

int grid_parse_json(const char *json, Grid *grid) {
    if (!json || !grid) {
        return -1;
    }

    if (grid_init(grid) != 0) {
        return -1;
    }

    return parse_grid_array(json, grid);
}

int grid_in_bounds(const Grid *grid, int row, int col) {
    return grid && row >= 0 && col >= 0 && row < grid->rows && col < grid->cols;
}

int grid_is_walkable(const Grid *grid, int row, int col) {
    char cell;

    if (!grid_in_bounds(grid, row, col)) {
        return 0;
    }

    cell = grid->cells[row][col];
    return cell != CELL_OBSTACLE && cell != CELL_FIRE;
}

int grid_is_exit(const Grid *grid, int row, int col) {
    if (!grid_in_bounds(grid, row, col)) {
        return 0;
    }

    return grid->cells[row][col] == CELL_EXIT;
}

void grid_free(Grid *grid) {
    if (!grid) {
        return;
    }

    grid_init(grid);
}
