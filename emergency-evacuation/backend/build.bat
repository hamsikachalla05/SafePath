@echo off
setlocal

cd /d "%~dp0"

where gcc >nul 2>&1
if errorlevel 1 (
    echo.
    echo [ERROR] gcc was not found in PATH.
    echo.
    echo Install MinGW-w64, then reopen this terminal:
    echo   https://www.mingw-w64.org/downloads/
    echo.
    echo Or install via MSYS2 and add mingw64\bin to PATH.
    echo.
    pause
    exit /b 1
)

echo Building SafePath evacuation engine...
gcc -Wall -Wextra -std=c11 -O2 -o evacuation.exe main.c grid.c bfs.c simulation.c
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo Building test runner...
gcc -Wall -Wextra -std=c11 -O2 -o run_tests.exe tests.c grid.c bfs.c
if errorlevel 1 (
    echo.
    echo [ERROR] Test build failed.
    pause
    exit /b 1
)

echo.
echo Running BFS tests...
run_tests.exe
if errorlevel 1 (
    echo.
    echo [ERROR] One or more tests failed.
    pause
    exit /b 1
)

echo.
echo [OK] evacuation.exe created successfully.
echo You can now run the Flask API and click Run Evacuation.
echo.
pause
