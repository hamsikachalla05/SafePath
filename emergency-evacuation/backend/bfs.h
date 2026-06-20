#ifndef BFS_H
#define BFS_H

#include "grid.h"

#define BFS_STATUS_SAFE "SAFE"
#define BFS_STATUS_NO_ROUTE "NO SAFE ROUTE FOUND"

typedef struct {
    Node path[MAX_PATH_LEN];
    int length;
    char status[32];
} BfsResult;

int bfs_find_path(const Grid *grid, BfsResult *result);

#endif
