# 5–8 Minute Presentation Outline
Keep the deck visual and explain the system rather than reading source code.

1. **Title + team (20 sec):** Smart Parking Management System, course, team and guide.
2. **Problem (35 sec):** Manual slot assignment, visibility and record consistency.
3. **Objectives and scope (35 sec):** What the prototype does and what it deliberately does not claim.
4. **Requirements (35 sec):** Entry/exit, categories, fees, occupancy, search, persistence.
5. **Architecture/UML (60 sec):** `Vehicle` hierarchy, `ParkingSlot`, `ParkingManager`, CSV storage.
6. **Algorithms (45 sec):** First-fit compatible slot allocation; rounded-up hourly billing.
7. **Live demo (90 sec):** Enter car → show slot/report → exit → receipt/history → restart persistence.
8. **Testing/results (45 sec):** Show completed 10+ case table, including one error/boundary case.
9. **Challenges, limitations, future scope (35 sec):** Explain CSV and no hardware integration honestly.
10. **Individual contributions (20 sec):** Evidence-backed roles and meaningful commits.

## Demo rehearsal checklist
- Compile from a clean checkout.
- Start from a clean or deliberately prepared data folder.
- Keep one valid entry, one successful exit and one error case ready.
- Verify output and persisted files before presenting.
- Do not claim hardware, AI, live sensors or payment integration.
- Rehearse within 5–8 minutes; expect two minutes of questions.
