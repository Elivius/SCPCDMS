# SCPCDMS

Smart Campus Parcel Collection and Delivery Management System, a C++ console program.
Parcels are registered, sorted, assigned to counters or couriers, and collected or returned.
Every data structure is written by hand (no STL containers).

| Task | Module | Data structure |
|---|---|---|
| 1 | `modules/ParcelIntake` | Queue |
| 2 | `modules/ServiceManager` | Priority Queue |
| 3 | `modules/ResourceManager` | Circular Queue |
| 4 | `modules/ActivityLog` | Stack |
| 5 | `modules/ParcelCategory` | Binary Search Tree (`ds/ParcelBST`) |

## Setup

Requirements: Visual Studio 2022 or newer with the "Desktop development with C++" workload.

1. Clone the repository and switch to your branch.
2. Open `SCPCDMS.slnx` in Visual Studio.
3. Set the configuration to Debug or Release, x64.
4. Build and run with Ctrl+F5.

Command line alternative (from a Developer Command Prompt):

```
cl /std:c++20 /EHsc /W4 SCPCDMS.cpp ds\ParcelBST.cpp modules\*.cpp Utils.cpp
```
