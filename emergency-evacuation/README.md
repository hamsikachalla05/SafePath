# SafePath – Intelligent Emergency Evacuation Simulator

A Programming in C project-based learning (PBL) application that finds the **shortest safe evacuation route** from a person to the nearest exit using **BFS**.

## Core Objective

Represent a building as a 2D grid and use Breadth-First Search (implemented entirely in C) to find the shortest path from `S` (person) to `E` (exit), avoiding obstacles and fire.

## Grid Symbols

| Symbol | Meaning |
|--------|---------|
| `S` | Person (start) |
| `E` | Exit (goal) |
| `X` | Obstacle (blocked) |
| `F` | Fire (blocked) |
| `.` | Empty cell |

## Architecture

```text
Frontend (HTML/CSS/JS)
        ↓
Flask API (Python bridge only)
        ↓
Compiled C Executable (BFS engine)
        ↓
JSON Result
        ↓
Frontend Visualization
```

All pathfinding logic lives in C. Python only forwards requests and returns JSON.

## Folder Structure

```text
emergency-evacuation/
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
├── api/
│   ├── app.py
│   └── requirements.txt
├── backend/
│   ├── main.c
│   ├── grid.c / grid.h
│   ├── bfs.c / bfs.h
│   ├── simulation.c / simulation.h
│   ├── tests.c
│   ├── Makefile
│   └── build.bat
└── README.md
```

## Build Instructions

### 1. Compile the C engine

**MSYS2 / Linux / macOS:**

```bash
cd backend
make
make test
```

**Windows (Command Prompt):**

```bat
cd backend
build.bat
```

This creates `evacuation.exe` and runs the built-in BFS test suite.

### 2. Install Python dependencies

```bash
cd api
pip install -r requirements.txt
```

### 3. Run the Flask server

```bash
cd api
python app.py
```

Open [http://127.0.0.1:5000](http://127.0.0.1:5000)

## BFS Test Cases

The file `backend/tests.c` includes:

**Test Case 1 – valid path**

```text
S . . .
. X . .
. . . .
. . . E
```

Expected: `SAFE` with a valid path.

**Test Case 2 – blocked grid**

```text
S X X
X X X
X X E
```

Expected: `NO SAFE ROUTE FOUND`

Run tests:

```bash
cd backend
make test
```

## API

### `GET /health`

Returns API and engine status.

### `POST /run-evacuation`

Runs the BFS evacuation simulation.

**Request**

```json
{
  "grid": [
    ["S", ".", ".", "E"],
    [".", "X", ".", "."],
    [".", ".", "F", "."]
  ]
}
```

**Response**

```json
{
  "path": [[0,0],[0,1],[0,2],[0,3]],
  "length": 4,
  "status": "SAFE",
  "algorithm": "BFS"
}
```

**Status values**

- `SAFE` – shortest safe route found
- `NO SAFE ROUTE FOUND` – no route avoids obstacles and fire
- `ENGINE NOT BUILT` – compile the C backend first

## BFS Explanation (for viva)

1. Enqueue the start cell and mark it visited.
2. Dequeue the front cell.
3. If the cell is an exit (`E`), reconstruct the path using parent pointers.
4. Otherwise enqueue all unvisited walkable neighbors (up, down, left, right).
5. Repeat until the queue is empty.

BFS explores layer by layer, so the first exit reached is on a shortest path.

### Data structures

- **Queue** – frontier of cells to explore
- **visited[]** – prevents revisiting cells
- **parent[]** – rebuilds the final route

## Frontend Usage

1. Set grid size and click **Apply Grid Size**.
2. Place Person, Exit, Obstacle, Fire, or Empty cells.
3. Click **Run Evacuation** to compute the shortest safe route.
4. Click **Reset Grid** to restore the demo layout.

## Troubleshooting

- **`ENGINE NOT BUILT`:** run `make` or `build.bat` inside `backend/`.
- **MSYS2 path error:** use `cd "/c/Users/.../backend"` with quotes.
- **False no-route results:** rebuild after code changes so `evacuation.exe` is up to date.

## What This Project Demonstrates

- Graph traversal with BFS
- Shortest-path pathfinding on a grid
- Emergency evacuation planning
- Modular C programming (grid, BFS, simulation layers)
- Integration between a C engine and a web frontend via Flask
