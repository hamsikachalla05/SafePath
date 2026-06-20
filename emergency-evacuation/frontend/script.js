const CELL_LABELS = {
    ".": "",
    S: "S",
    E: "E",
    X: "X",
    F: "F",
};

const CELL_CLASSES = {
    ".": "empty",
    S: "person",
    E: "exit",
    X: "obstacle",
    F: "fire",
};

const state = {
    rows: 8,
    cols: 10,
    grid: [],
    selectedTool: "S",
    activePath: [],
};

const gridContainer = document.getElementById("grid-container");
const gridRowsInput = document.getElementById("grid-rows");
const gridColsInput = document.getElementById("grid-cols");
const gridMeta = document.getElementById("grid-meta");
const resultAlgorithm = document.getElementById("result-algorithm");
const resultStatus = document.getElementById("result-status");
const resultLength = document.getElementById("result-length");
const resultPath = document.getElementById("result-path");
const resultMessage = document.getElementById("result-message");
const engineAlert = document.getElementById("engine-alert");
const runEvacuationButton = document.getElementById("run-evacuation");

function createEmptyGrid(rows, cols) {
    return Array.from({ length: rows }, () => Array(cols).fill("."));
}

function initializeDemoLayout() {
    state.grid = createEmptyGrid(state.rows, state.cols);
    state.grid[0][0] = "S";
    state.grid[0][3] = "E";
    state.grid[1][1] = "X";
    state.grid[2][1] = "X";
    state.grid[2][2] = "F";
    state.activePath = [];
}

function renderGrid() {
    gridContainer.style.gridTemplateColumns = `repeat(${state.cols}, 42px)`;
    gridContainer.innerHTML = "";

    for (let row = 0; row < state.rows; row++) {
        for (let col = 0; col < state.cols; col++) {
            const cellValue = state.grid[row][col];
            const button = document.createElement("button");
            const isOnPath = state.activePath.some(
                ([pathRow, pathCol]) => pathRow === row && pathCol === col
            );

            button.type = "button";
            button.className = `grid-cell ${isOnPath ? "path" : CELL_CLASSES[cellValue]}`;
            button.textContent = isOnPath ? "•" : CELL_LABELS[cellValue];
            button.title = `Row ${row}, Column ${col}`;
            button.addEventListener("click", () => placeCell(row, col));
            gridContainer.appendChild(button);
        }
    }

    gridMeta.textContent = `${state.rows} x ${state.cols} grid`;
}

function placeCell(row, col) {
    const tool = state.selectedTool;

    if (tool === "S" || tool === "E") {
        for (let r = 0; r < state.rows; r++) {
            for (let c = 0; c < state.cols; c++) {
                if (state.grid[r][c] === tool) {
                    state.grid[r][c] = ".";
                }
            }
        }
    }

    state.grid[row][col] = tool;
    state.activePath = [];
    clearResults();
    renderGrid();
}

function resizeGrid() {
    const rows = Number(gridRowsInput.value);
    const cols = Number(gridColsInput.value);

    if (rows < 3 || rows > 20 || cols < 3 || cols > 20) {
        alert("Grid size must be between 3 and 20 for both rows and columns.");
        return;
    }

    state.rows = rows;
    state.cols = cols;
    initializeDemoLayout();
    clearResults();
    renderGrid();
}

function resetGrid() {
    initializeDemoLayout();
    clearResults();
    renderGrid();
}

function clearResults() {
    resultAlgorithm.textContent = "BFS";
    resultStatus.textContent = "Waiting";
    resultStatus.className = "result-value status-pill";
    resultLength.textContent = "—";
    resultPath.textContent = "Run evacuation to see the shortest safe route.";
    resultMessage.textContent = "";
    resultMessage.classList.remove("error");
}

function updateResults(data) {
    resultAlgorithm.textContent = data.algorithm || "BFS";
    resultLength.textContent = String(data.length ?? 0);
    resultStatus.textContent = data.status || "UNKNOWN";
    resultStatus.className = "result-value status-pill";

    if (data.status === "SAFE") {
        resultStatus.classList.add("safe");
    } else {
        resultStatus.classList.add("danger");
    }

    if (data.error) {
        resultMessage.textContent = data.error;
        resultMessage.classList.add("error");
    } else {
        resultMessage.textContent = "";
        resultMessage.classList.remove("error");
    }

    if (Array.isArray(data.path) && data.path.length > 0) {
        resultPath.textContent = data.path
            .map(([row, col]) => `[${row}, ${col}]`)
            .join(" -> ");
        state.activePath = data.path;
    } else {
        resultPath.textContent = "No evacuation route available.";
        state.activePath = [];
    }

    renderGrid();
}

async function checkEngineHealth() {
    try {
        const response = await fetch("/health");
        const data = await response.json();

        if (!data.engine_ready) {
            engineAlert.classList.remove("hidden");
            engineAlert.innerHTML =
                "<strong>C engine not built.</strong> Open the <code>backend</code> folder, run <code>build.bat</code> or <code>make</code>, then restart Flask.";
            return;
        }

        engineAlert.classList.add("hidden");
    } catch (error) {
        engineAlert.classList.remove("hidden");
        engineAlert.textContent = "Could not reach the API health endpoint.";
    }
}

async function runEvacuation() {
    runEvacuationButton.disabled = true;

    try {
        const response = await fetch("/run-evacuation", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ grid: state.grid }),
        });

        const data = await response.json();
        updateResults(data);
    } catch (error) {
        updateResults({
            status: "NO SAFE ROUTE FOUND",
            length: 0,
            path: [],
            algorithm: "BFS",
            error: `Request failed: ${error.message}`,
        });
    } finally {
        runEvacuationButton.disabled = false;
    }
}

document.querySelectorAll(".tool-btn").forEach((button) => {
    button.addEventListener("click", () => {
        document.querySelectorAll(".tool-btn").forEach((item) => {
            item.classList.remove("active");
        });
        button.classList.add("active");
        state.selectedTool = button.dataset.tool;
    });
});

document.getElementById("resize-grid").addEventListener("click", resizeGrid);
document.getElementById("reset-grid").addEventListener("click", resetGrid);
runEvacuationButton.addEventListener("click", runEvacuation);

initializeDemoLayout();
renderGrid();
checkEngineHealth();
