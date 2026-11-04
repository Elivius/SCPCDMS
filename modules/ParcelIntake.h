#pragma once
// Task 1 - Parcel Intake, Sorting and Processing
// Uses: Queue<std::string> (ds/Queue.h) - IDs of parcels waiting to be sorted, oldest arrival first.
//       Parcel records are kept once, in ParcelCategory's BST (Task 5); look them up by ID.
// Covers: register a parcel, display/search records, sort and route incoming parcels,
//         update/remove records, exceptional parcel conditions.
// Works with: ParcelCategory (records), ServiceManager (request after sorting), ActivityLog (history).

#include "../Models.h"
#include "../ds/Queue.h"

class ParcelIntake {
};
