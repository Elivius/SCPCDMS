#pragma once
// Task 4 - Parcel Activity and Record Management
// Uses: Stack<Activity> (ds/Stack.h) - activity history; the most recent activity is on top,
//       which is where undo starts.
// Covers: record activities, show recent and per-parcel history, review past activities,
//         undo an eligible recent activity, activity-history exceptions.
// Works with: every module records here; undo restores status in ParcelCategory, ServiceManager
//             and ResourceManager.

#include "../Models.h"
#include "../ds/Stack.h"

class ActivityLog {
};
