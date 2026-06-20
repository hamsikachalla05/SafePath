#include "simulation.h"

#include <stdio.h>
#include <string.h>

int simulation_run(const Grid *grid, SimulationResult *result) {
    BfsResult bfs_result;

    if (!grid || !result) {
        return -1;
    }

    bfs_find_path(grid, &bfs_result);

    result->length = bfs_result.length;
    for (int i = 0; i < bfs_result.length; i++) {
        result->path[i] = bfs_result.path[i];
    }

    strncpy(result->status, bfs_result.status, sizeof(result->status) - 1);
    result->status[sizeof(result->status) - 1] = '\0';

    return 0;
}

int simulation_result_to_json(const SimulationResult *result, char *buffer, int buffer_size) {
    int written = 0;
    int offset = 0;

    if (!result || !buffer || buffer_size <= 0) {
        return -1;
    }

    written = snprintf(buffer + offset, buffer_size - offset, "{\"path\":[");
    if (written < 0 || written >= buffer_size - offset) {
        return -1;
    }
    offset += written;

    for (int i = 0; i < result->length; i++) {
        written = snprintf(
            buffer + offset,
            buffer_size - offset,
            "%s[%d,%d]",
            i == 0 ? "" : ",",
            result->path[i].row,
            result->path[i].col
        );
        if (written < 0 || written >= buffer_size - offset) {
            return -1;
        }
        offset += written;
    }

    written = snprintf(
        buffer + offset,
        buffer_size - offset,
        "],\"length\":%d,\"status\":\"%s\",\"algorithm\":\"BFS\"}",
        result->length,
        result->status
    );
    if (written < 0 || written >= buffer_size - offset) {
        return -1;
    }

    return 0;
}
