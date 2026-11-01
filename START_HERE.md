# Start Here — Smart Parking Management System

This folder contains **two runnable demonstrations**:

1. **C++17 console application** — the OOP implementation for the course project. It saves records in `data/`.
2. **ParkPilot browser UI** — a separate interactive visual demo. It saves its own demo records in your browser's local storage. It is **not connected to the C++ program**; do not describe it as a single integrated frontend/backend system.

## Run the UI (no installation)
- Windows: double-click `OPEN_UI.bat`, or double-click `parkpilot_dashboard.html`.
- macOS/Linux: open `parkpilot_dashboard.html` in a modern browser.
- Use **Vehicle entry** to register a vehicle, click a parking tile to inspect it, switch to **Vehicle exit** to calculate a fee, search activity, export CSV, and print the receipt.
- UI data stays in the same browser on that device. Use **Reset demo** to clear it.

## Run the C++ program
### Windows
1. Install a C++17 compiler such as MinGW-w64/MSYS2 and make sure `g++` is available in PATH.
2. Double-click `RUN_WINDOWS.bat`.

### Linux
Install g++ if required, then run from this directory:
```bash
chmod +x RUN_LINUX_MAC.sh
./RUN_LINUX_MAC.sh
```

### Compile manually
```bash
g++ -std=c++17 -Wall -Wextra -pedantic SourceCode/main.cpp SourceCode/Vehicle.cpp SourceCode/ParkingSlot.cpp SourceCode/ParkingManager.cpp -o smart_parking
./smart_parking
```
On Windows, change the output to `-o smart_parking.exe` and run `smart_parking.exe`.

## Folder map
- `SourceCode/` — C++ classes and application entry point
- `data/` — CSV persistence used by the C++ console app
- `docs/` — proposal, requirements, design, test report, final report template and viva preparation
- `parkpilot_dashboard.html` — self-contained browser UI
- `Presentation.pptx` — project presentation
- `README.md` — detailed features, assumptions and OOP mapping

## First demo sequence
1. Open the UI and register a Car with a sample registration such as `JH01AB1234`.
2. Show its assigned space and updated occupancy.
3. Check it out and show the generated fee/receipt.
4. Search the activity and export CSV.
5. Separately run the C++ console program and demonstrate its menu-based workflow.

## Important
This is an academic prototype. Slot availability is simulated; there are no physical sensors, live payments, authentication, or database server. The browser UI and C++ console app currently maintain separate data stores.
