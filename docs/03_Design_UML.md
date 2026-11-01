# System Design and UML
## Architecture
```text
Operator
   |
   v
Console Menu (main.cpp)
   |
   v
ParkingManager ----------------------> CSV files
   |                                  slots.csv
   |                                  active.csv
   |                                  history.csv
   +---- owns ----> ParkingSlot[]
   |
   +---- owns ----> ActiveParking[]
                         |
                         v
                    Vehicle (abstract)
                    /       |        \
                  Car   Motorcycle   Truck
```

## Class relationships
- `Car`, `Motorcycle`, `Truck` inherit from abstract `Vehicle`.
- `ParkingManager` aggregates a vector of `ParkingSlot` objects and owns active vehicle objects using `std::unique_ptr`.
- `ActiveParking` associates one vehicle with a slot ID and entry timestamp.
- `ParkingManager` stores completed `ParkingReceipt` records.

## Core algorithms
### Entry
1. Validate registration and owner.
2. Reject if the registration already has an active session.
3. Construct the appropriate derived `Vehicle`.
4. Find the first free slot matching `requiredSlotType()`.
5. If no slot matches, report a clear error.
6. Occupy slot, record entry time, persist state and return slot ID.

### Exit
1. Find the active registration.
2. Reject unknown registrations or exit time earlier than entry.
3. Compute elapsed minutes and `max(1, ceil(elapsed seconds / 3600))`.
4. Multiply billable hours by the vehicle's virtual hourly rate.
5. Release slot, append receipt, remove active session and persist state.

## Complexity (n = slots, a = active sessions, h = history)
- Allocation: O(n + a) in the current straightforward implementation.
- Active lookup: O(a).
- Slot display/occupancy report: O(n).
- History display/search: O(h).
- Storage: O(n + a + h).
A future indexed design could use `unordered_map` for registration lookups and a database for transactional persistence.
