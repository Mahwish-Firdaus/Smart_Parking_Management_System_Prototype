# Smart Parking Management System
**SBU B.Tech CSE Group Project | OOP Using C++ | C++17**

A modular, console-based prototype that allocates compatible parking slots, records vehicle entry/exit, calculates parking fees, reports occupancy, searches parking records, and persists data to CSV files across restarts.

## Features
- Vehicle categories: Car, Motorcycle/Bike, Truck.
- Slot categories: Standard, TwoWheeler, Heavy.
- First-fit automatic allocation to a compatible free slot.
- Duplicate active-registration prevention and full-lot handling.
- Fee calculation: Car ₹30/hour, Motorcycle ₹15/hour, Truck ₹60/hour. Any started hour is charged as a full hour; minimum fee is one hour.
- Occupancy summary by category and overall.
- Active vehicles, search, completed parking history and receipt.
- CSV persistence for slots, active sessions and completed history.
- Input validation, exceptions and malformed-row skipping.
- OOP hierarchy with abstract base class and virtual functions.

## Requirements
- A C++17 compiler (GCC 8+, Clang 7+, or a compatible IDE).
- `std::filesystem` support.

## Build and run
From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic SourceCode/main.cpp SourceCode/Vehicle.cpp SourceCode/ParkingSlot.cpp SourceCode/ParkingManager.cpp -o smart_parking
./smart_parking
```

Windows (MinGW):
```bash
g++ -std=c++17 -Wall -Wextra -pedantic SourceCode/main.cpp SourceCode/Vehicle.cpp SourceCode/ParkingSlot.cpp SourceCode/ParkingManager.cpp -o smart_parking.exe
smart_parking.exe
```

The program creates a `data/` directory in the current working directory. It contains `slots.csv`, `active.csv`, and `history.csv`. Keep these files together to preserve state. For a clean demo, back up and then remove the `data/` directory while the application is closed.

## Suggested end-to-end demo
1. Choose `1`; enter `Car`, `JH01AB1234`, `Aarav Sharma`.
2. Choose `3` to show the assigned Standard slot is occupied.
3. Choose `5` to show updated occupancy.
4. Choose `4` to show the active vehicle.
5. Choose `2`; enter `JH01AB1234`; show the receipt and calculated fee.
6. Choose `7` to show completed history.
7. Exit, restart, and show that the completed history and slot state persist.
8. Demonstrate an error: try to exit a registration that is not currently parked.

## Fee and allocation assumptions
These are prototype assumptions, not official real-world tariffs. Slot compatibility is exact: Car → Standard, Motorcycle → TwoWheeler, Truck → Heavy. Allocation uses the first available compatible slot. Parking fee is rounded up to a whole hour, with a one-hour minimum. Real deployments should make tariffs configurable and add date/time policy, authentication, payment handling and concurrency control.

## OOP mapping
| Requirement | Implementation |
|---|---|
| Classes/objects | `Vehicle`, `Car`, `Motorcycle`, `Truck`, `ParkingSlot`, `ParkingManager` |
| Encapsulation | Private slot and manager state; public operations |
| Inheritance | Three vehicle subclasses inherit `Vehicle` |
| Runtime polymorphism | Virtual `type`, `requiredSlotType`, `hourlyRate` |
| Abstraction | Abstract `Vehicle` interface |
| Constructors/destructors | Constructors validate/initialise; virtual base destructor |
| STL | `std::vector`, `std::unique_ptr`, algorithms |
| Exceptions | Invalid category, duplicate registration, full lot, unknown vehicle, invalid exit |
| File persistence | CSV files in `data/` |
| Modular code | Separate headers and implementation files |

## Limitations
- Console interface, not a sensor-connected IoT deployment.
- No real payment gateway, user authentication, reservations, or simultaneous multi-process safety.
- CSV persistence is appropriate for a small academic prototype; a production-scale system should use a database and transactional updates.
- Vehicle registrations and owner names are case-sensitive.
- CSV fields are sanitised by replacing commas/newlines; this is not a general CSV parser.

## Academic integrity
Review and understand every module, adapt the design with your team, run the tests, and disclose AI assistance if required by the instructor. Do not claim a test passed unless you have run it on the submitted build.
