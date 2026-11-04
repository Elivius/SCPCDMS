#pragma once
// Task 2 - Parcel Service and Collection Management
// Uses: PriorityQueue<ServiceRequest> (ds/PriorityQueue.h) - active requests; top() is handled next.
//       operator< for ServiceRequest follows the team's priority rule (README) and falls back to
//       sequenceNo, so requests with equal priority are first-come, first-served.
// Covers: create a request, list active requests, pick the next request,
//         process/update requests, exceptional service conditions.
// Works with: ParcelCategory (parcel status), ResourceManager (assignment), ActivityLog (history).

#include "../Models.h"
#include "../ds/PriorityQueue.h"

class ServiceManager {
};
