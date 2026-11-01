@echo off
setlocal
cd /d "%~dp0"
where g++ >nul 2>nul
if errorlevel 1 (
  echo ERROR: g++ was not found. Install MinGW-w64 or MSYS2 and add its bin folder to PATH.
  echo You can still run the browser UI by double-clicking parkpilot_dashboard.html.
  pause
  exit /b 1
)
echo Building Smart Parking Management System...
g++ -std=c++17 -Wall -Wextra -pedantic SourceCode\main.cpp SourceCode\Vehicle.cpp SourceCode\ParkingSlot.cpp SourceCode\ParkingManager.cpp -o smart_parking.exe
if errorlevel 1 (echo Build failed. Check compiler errors above.&pause&exit /b 1)
echo Build successful. Starting console application...
smart_parking.exe
pause
