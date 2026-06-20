#ifndef SIMULATION_H
#define SIMULATION_H

#include "bfs.h"
#include "grid.h"

typedef struct {
    Node path[MAX_PATH_LEN];
    int length;
    char status[32];
} SimulationResult;

int simulation_run(const Grid *grid, SimulationResult *result);
int simulation_result_to_json(const SimulationResult *result, char *buffer, int buffer_size);

#endif
