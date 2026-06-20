#include "simulation.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 65536
#define MAX_OUTPUT_SIZE 16384

static int read_stdin(char *buffer, int buffer_size) {
    int total = 0;
    int chunk;

    while (total < buffer_size - 1) {
        chunk = (int)fread(buffer + total, 1, buffer_size - 1 - total, stdin);
        if (chunk <= 0) {
            break;
        }
        total += chunk;
    }

    buffer[total] = '\0';
    return total > 0 ? 0 : -1;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char output[MAX_OUTPUT_SIZE];
    Grid grid;
    SimulationResult result;

    if (read_stdin(input, MAX_INPUT_SIZE) != 0) {
        fprintf(stderr, "Failed to read input JSON from stdin.\n");
        return 1;
    }

    if (grid_parse_json(input, &grid) != 0) {
        fprintf(stderr, "Failed to parse grid JSON.\n");
        return 1;
    }

    if (simulation_run(&grid, &result) != 0) {
        fprintf(stderr, "Simulation failed.\n");
        grid_free(&grid);
        return 1;
    }

    if (simulation_result_to_json(&result, output, MAX_OUTPUT_SIZE) != 0) {
        fprintf(stderr, "Failed to serialize simulation result.\n");
        grid_free(&grid);
        return 1;
    }

    printf("%s\n", output);
    grid_free(&grid);
    return 0;
}
