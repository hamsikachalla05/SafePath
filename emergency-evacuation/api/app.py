import json
import os
import subprocess
from pathlib import Path

from flask import Flask, jsonify, request, send_from_directory

BASE_DIR = Path(__file__).resolve().parent.parent
BACKEND_DIR = BASE_DIR / "backend"
FRONTEND_DIR = BASE_DIR / "frontend"

if os.name == "nt":
    EXECUTABLE = BACKEND_DIR / "evacuation.exe"
else:
    EXECUTABLE = BACKEND_DIR / "evacuation"

app = Flask(__name__, static_folder=str(FRONTEND_DIR), static_url_path="")


def run_evacuation(grid_payload: dict):
    if not EXECUTABLE.exists():
        return {
            "error": (
                "C engine not built. Open a terminal in the backend folder and run "
                "build.bat (Windows) or make (Linux/macOS), then restart Flask."
            ),
            "status": "ENGINE NOT BUILT",
            "path": [],
            "length": 0,
            "algorithm": "BFS",
        }, 503

    payload = json.dumps(grid_payload)
    try:
        completed = subprocess.run(
            [str(EXECUTABLE)],
            input=payload,
            text=True,
            capture_output=True,
            check=False,
            cwd=str(BACKEND_DIR),
        )
    except OSError as exc:
        return {
            "error": f"Failed to execute simulation engine: {exc}",
            "status": "NO SAFE ROUTE FOUND",
            "path": [],
            "length": 0,
            "algorithm": "BFS",
        }, 500

    if completed.returncode != 0:
        return {
            "error": completed.stderr.strip() or "Simulation engine returned a non-zero exit code.",
            "status": "NO SAFE ROUTE FOUND",
            "path": [],
            "length": 0,
            "algorithm": "BFS",
        }, 500

    try:
        return json.loads(completed.stdout.strip()), 200
    except json.JSONDecodeError:
        return {
            "error": "Simulation engine returned invalid JSON.",
            "raw_output": completed.stdout.strip(),
            "status": "NO SAFE ROUTE FOUND",
            "path": [],
            "length": 0,
            "algorithm": "BFS",
        }, 500


@app.route("/")
def serve_index():
    return send_from_directory(FRONTEND_DIR, "index.html")


@app.route("/health", methods=["GET"])
def health():
    return jsonify(
        {
            "status": "ok",
            "engine_ready": EXECUTABLE.exists(),
            "engine_path": str(EXECUTABLE),
        }
    )


@app.route("/run-evacuation", methods=["POST"])
def run_evacuation_route():
    data = request.get_json(silent=True) or {}
    if "grid" not in data:
        return jsonify({"error": "Request body must include a 'grid' field."}), 400

    result, status_code = run_evacuation(data)
    return jsonify(result), status_code


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
