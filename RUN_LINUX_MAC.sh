#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"
command -v g++ >/dev/null 2>&1 || { echo "Install g++ first (for example: sudo apt install g++)."; exit 1; }
g++ -std=c++17 -Wall -Wextra -pedantic SourceCode/main.cpp SourceCode/Vehicle.cpp SourceCode/ParkingSlot.cpp SourceCode/ParkingManager.cpp -o smart_parking
exec ./smart_parking
