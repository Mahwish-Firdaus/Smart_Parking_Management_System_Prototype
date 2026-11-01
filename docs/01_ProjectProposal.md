# Project Proposal: Smart Parking Management System
**Course:** Object-Oriented Programming Using C++ | **Academic Session:** 2026–27

## 1. Problem
Manual parking allocation can waste time, make it difficult to know which slots are free, and lead to inconsistent fee records. This prototype models a small parking facility with category-aware slot assignment and persistent parking records.

## 2. Objectives
1. Allocate a compatible available slot when a vehicle enters.
2. Prevent duplicate active registrations and reject unsupported vehicle types.
3. Record entry and exit and calculate a transparent fee.
4. Show live slot status, active vehicles and category-wise occupancy.
5. Preserve slot state, active sessions and completed receipts across restart.
6. demonstrate OOP concepts using a modular C++17 design.
7. Validate correctness with documented normal, boundary and error tests.

## 3. Scope
Included: cars, motorcycles and trucks; Standard, TwoWheeler and Heavy slots; automatic allocation; exit receipt; occupancy and history reports; file persistence.

Out of scope: physical sensors, camera/ANPR, online payment, user accounts, cloud hosting, reservations and simultaneous multi-process operation.

## 4. Team roles (replace placeholders)
- Member 1 — Architecture/integration: [Name]
- Member 2 — Vehicle hierarchy and allocation: [Name]
- Member 3 — Persistence/reporting/UI: [Name]
- Member 4 — Test design, documentation and demo: [Name]
Each member should make traceable commits and understand the full codebase.

## 5. Initial architecture
Menu/UI → ParkingManager → Vehicle abstraction and ParkingSlot → CSV persistence.
ParkingManager owns the slot inventory, active sessions and completed receipt history.

## 6. Success criteria
A vehicle receives only a compatible free slot; duplicate active entries and invalid exits are rejected; fees follow the documented rule; reports match state; and saved data reloads after restarting.
