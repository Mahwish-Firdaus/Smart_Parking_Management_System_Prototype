# Test Report
**Important:** Expected results are specified below. Fill in Actual Result and Status after executing each scenario on the final build; do not mark a case passed without evidence.

| ID | Input/scenario | Expected result | Actual result | Status |
|---|---|---|---|---|
| TC-01 | Park a Car with a unique registration | Allocated a Standard slot | Pending execution | Not run |
| TC-02 | Park a Motorcycle | Allocated a TwoWheeler slot | Pending execution | Not run |
| TC-03 | Park a Truck | Allocated a Heavy slot | Pending execution | Not run |
| TC-04 | Enter same registration twice while active | Second entry rejected; first remains active | Pending execution | Not run |
| TC-05 | Unsupported vehicle type | Clear error; no slot state change | Pending execution | Not run |
| TC-06 | Exit unknown registration | Clear error; no slot state change | Pending execution | Not run |
| TC-07 | Add invalid slot category | Rejected; inventory unchanged | Pending execution | Not run |
| TC-08 | Exit valid vehicle | Slot released and receipt stored | Pending execution | Not run |
| TC-09 | Restart after entry/exit | Slots and history reload from CSV | Pending execution | Not run |
| TC-10 | Fill all compatible slots, then add one more vehicle of that type | Full-category error; no duplicate assignment | Pending execution | Not run |
| TC-11 | Exit timestamp earlier than entry (unit-level/manual test) | Rejected without removing active session | Pending execution | Not run |
| TC-12 | Invalid menu input such as `hello` | Error displayed; application continues | Pending execution | Not run |
| TC-13 | Occupancy report after entry and exit | Counts agree with slot directory | Pending execution | Not run |
| TC-14 | Empty data directory / first launch | Default inventory initialised | Pending execution | Not run |

## Evidence checklist
- Capture a screenshot of successful entry and allocated slot.
- Capture slot directory and occupancy report.
- Capture successful exit receipt.
- Capture at least one rejected operation.
- Restart the program and show persisted state.
- Record compiler version, build command and date of test.
