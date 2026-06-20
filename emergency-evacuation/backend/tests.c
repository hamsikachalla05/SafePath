#include <stdio.h>
#include <string.h>

#include "bfs.h"
#include "grid.h"

static int load_grid_from_rows(Grid *grid, const char *rows[], int row_count, int col_count) {
    if (grid_init(grid) != 0) {
        return -1;
    }

    grid->rows = row_count;
    grid->cols = col_count;

    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            char cell = rows[row][col];
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
    }

    return 0;
}

static int run_test(const char *name, const char *rows[], int row_count, int col_count, int expect_safe) {
    Grid grid;
    BfsResult result;
    int passed;

    load_grid_from_rows(&grid, rows, row_count, col_count);
    bfs_find_path(&grid, &result);

    if (expect_safe) {
        passed = strcmp(result.status, BFS_STATUS_SAFE) == 0 && result.length > 0;
    } else {
        passed = strcmp(result.status, BFS_STATUS_NO_ROUTE) == 0 && result.length == 0;
    }

    printf("[%s] %s\n", passed ? "PASS" : "FAIL", name);
    if (!passed) {
        printf("  expected: %s\n", expect_safe ? "SAFE path" : "NO SAFE ROUTE FOUND");
        printf("  actual:   %s (length=%d)\n", result.status, result.length);
    }

    grid_free(&grid);
    return passed ? 0 : 1;
}

int main(void) {
    int failures = 0;

    const char *test_case_1[] = {
        "S...",
        ".X..",
        "....",
        "...E",
    };

    const char *test_case_2[] = {
        "SXX",
        "XXX",
        "XXE",
    };

    printf("SafePath BFS test suite\n");
    printf("=======================\n");

    failures += run_test(
        "Test Case 1 - valid path around obstacle",
        test_case_1,
        4,
        4,
        1
    );

    failures += run_test(
        "Test Case 2 - fully blocked grid",
        test_case_2,
        3,
        3,
        0
    );

    printf("=======================\n");
    if (failures == 0) {
        printf("All tests passed.\n");
        return 0;
    }

    printf("%d test(s) failed.\n", failures);
    return 1;
}
