# Viva Preparation: Questions and Short Answer Guides

1. **Why is `Vehicle` abstract?** Different vehicle types share registration/owner data but need their own type, slot category and hourly rate.
2. **Where is inheritance used?** `Car`, `Motorcycle` and `Truck` derive from `Vehicle`.
3. **Where is runtime polymorphism?** `type()`, `requiredSlotType()` and `hourlyRate()` are virtual and overridden; `ParkingManager` calls them through a `Vehicle` pointer.
4. **Why is the destructor virtual?** A base pointer may own a derived object; virtual destruction ensures correct derived cleanup.
5. **Where is encapsulation used?** `ParkingSlot` state is private and changed through `park()`/`release()`; manager state is private.
6. **Why `unique_ptr`?** It expresses unique ownership and automatically frees a vehicle when its active session is erased.
7. **Why vectors?** They are simple STL containers for small inventories and records; scans are easy to explain.
8. **How does allocation work?** Scan slots in order and select the first unoccupied slot whose category matches the vehicle requirement.
9. **How is fee calculated?** `max(1, ceil(elapsed_seconds / 3600)) × hourly_rate`.
10. **What exceptions are handled?** Unsupported type, duplicate active registration, full compatible category, invalid slot category and invalid exit.
11. **How is persistence implemented?** `slots.csv`, `active.csv`, `history.csv` are written and read using file streams.
12. **What if a data row is malformed?** The loader catches conversion/validation errors and skips that row; production software should log the rejected row.
13. **What is the time complexity of allocation?** O(n) slot scan plus O(a) duplicate lookup, so O(n+a).
14. **How would you scale to 100,000 records?** Index registrations with `unordered_map`, use a database with transactions, and add concurrency controls.
15. **What is not implemented?** Physical sensors, ANPR, reservations, authentication, online payment and concurrent multi-process safety.
16. **Why not call it a complete IoT system?** This submission is a software simulation; hardware integration would require actual devices and testing.

Each student should be able to trace `parkVehicle`, `exitVehicle`, the derived vehicle methods and `saveAll/loadAll` without relying on slides.
