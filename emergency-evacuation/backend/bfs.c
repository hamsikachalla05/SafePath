#include "bfs.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    Node items[MAX_PATH_LEN];
    int front;
    int rear;
    int size;
} Queue;

static void queue_init(Queue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

static int queue_is_empty(const Queue *queue) {
    return queue->size == 0;
}

static int queue_enqueue(Queue *queue, Node node) {
    if (queue->size >= MAX_PATH_LEN) {
        return -1;
    }

    queue->rear = (queue->rear + 1) % MAX_PATH_LEN;
    queue->items[queue->rear] = node;
    queue->size++;
    return 0;
}

static int queue_dequeue(Queue *queue, Node *node) {
    if (queue_is_empty(queue)) {
        return -1;
    }

    *node = queue->items[queue->front];
    queue->front = (queue->front + 1) % MAX_PATH_LEN;
    queue->size--;
    return 0;
}

static int encode_index(const Grid *grid, int row, int col) {
    return row * grid->cols + col;
}

static void decode_index(const Grid *grid, int index, int *row, int *col) {
    *row = index / grid->cols;
    *col = index % grid->cols;
}

static void set_bfs_failure(BfsResult *result) {
    result->length = 0;
    strncpy(result->status, BFS_STATUS_NO_ROUTE, sizeof(result->status) - 1);
    result->status[sizeof(result->status) - 1] = '\0';
}

static int reconstruct_path(
    const Grid *grid,
    const int *parent,
    int end_index,
    BfsResult *result
) {
    int path_indices[MAX_PATH_LEN];
    int path_count = 0;
    int current = end_index;

    while (current != -1 && path_count < MAX_PATH_LEN) {
        path_indices[path_count++] = current;
        current = parent[current];
    }

    if (path_count == 0) {
        set_bfs_failure(result);
        return -1;
    }

    result->length = path_count;
    for (int i = 0; i < path_count; i++) {
        int row;
        int col;
        decode_index(grid, path_indices[path_count - 1 - i], &row, &col);
        result->path[i].row = row;
        result->path[i].col = col;
    }

    strncpy(result->status, BFS_STATUS_SAFE, sizeof(result->status) - 1);
    result->status[sizeof(result->status) - 1] = '\0';
    return 0;
}

int bfs_find_path(const Grid *grid, BfsResult *result) {
    int visited[MAX_GRID_ROWS * MAX_GRID_COLS];
    int parent[MAX_GRID_ROWS * MAX_GRID_COLS];
    Queue queue;
    int total_cells;
    int start_index;
    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    if (!grid || !result) {
        return -1;
    }

    result->length = 0;

    if (!grid->has_start || !grid->has_exit) {
        set_bfs_failure(result);
        return -1;
    }

    total_cells = grid->rows * grid->cols;
    for (int i = 0; i < total_cells; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    start_index = encode_index(grid, grid->start.row, grid->start.col);

    if (!grid_is_walkable(grid, grid->start.row, grid->start.col)) {
        set_bfs_failure(result);
        return -1;
    }

    queue_init(&queue);
    visited[start_index] = 1;
    parent[start_index] = -1;
    queue_enqueue(&queue, grid->start);

    while (!queue_is_empty(&queue)) {
        Node current;
        int current_index;

        queue_dequeue(&queue, &current);
        current_index = encode_index(grid, current.row, current.col);

        if (grid_is_exit(grid, current.row, current.col)) {
            return reconstruct_path(grid, parent, current_index, result);
        }

        for (int i = 0; i < 4; i++) {
            int next_row = current.row + directions[i][0];
            int next_col = current.col + directions[i][1];
            int next_index;

            if (!grid_is_walkable(grid, next_row, next_col)) {
                continue;
            }

            next_index = encode_index(grid, next_row, next_col);
            if (visited[next_index]) {
                continue;
            }

            visited[next_index] = 1;
            parent[next_index] = encode_index(grid, current.row, current.col);
            queue_enqueue(&queue, (Node){next_row, next_col});
        }
    }

    set_bfs_failure(result);
    return -1;
}
