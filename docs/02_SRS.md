# Software Requirements Specification (SRS)
## 1. Purpose
Define the requirements for a small C++17 smart parking management prototype.

## 2. Users
- Parking operator: registers entry/exit, views slots and reports.
- Demonstrator/admin: adds a slot for configuration and test scenarios.

## 3. Functional requirements
- FR-01: Display a menu for entry, exit, slots, active vehicles, occupancy, search, history, add slot and exit.
- FR-02: Accept Car, Motorcycle/Bike or Truck.
- FR-03: Allocate first free compatible slot: Car→Standard, Motorcycle→TwoWheeler, Truck→Heavy.
- FR-04: Reject duplicate active registration.
- FR-05: Reject entry when no compatible slot exists.
- FR-06: Reject exit when no active registration exists.
- FR-07: Calculate a fee from the vehicle's hourly rate, rounding any started hour upward and charging at least one hour.
- FR-08: Release the slot on successful exit and retain a receipt.
- FR-09: Report overall and category occupancy.
- FR-10: Search active and completed records.
- FR-11: Persist slots, active sessions and history to CSV and load them at startup.
- FR-12: Validate slot categories and handle invalid operations without terminating the menu.

## 4. Non-functional requirements
- NFR-01: C++17, standard library only.
- NFR-02: Modular headers and implementation files.
- NFR-03: Human-readable output and meaningful error messages.
- NFR-04: Persisted rows that are malformed are skipped rather than crashing startup.
- NFR-05: Compile using documented reproducible instructions.

## 5. Data model
- Vehicle: registration, owner; virtual type/category/rate interface.
- ParkingSlot: ID, category, occupied state and registration.
- ActiveParking: owned Vehicle object, slot ID, entry timestamp.
- ParkingReceipt: registration, type, slot ID, duration, fee and timestamps.
- ParkingManager: slots, active sessions, history and persistence paths.

## 6. Assumptions/constraints
The application is a single-process console simulation. Default inventory is 8 Standard, 5 TwoWheeler and 2 Heavy slots. Tariffs are prototype values: ₹30, ₹15 and ₹60 per hour respectively. No hardware sensors, network services or payment gateway are included.
