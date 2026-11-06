#pragma once
// Shared records used by every module: Parcel, ServiceRequest, Resource, Activity + status/type enums.
// Fields and values follow the assignment brief; anything the brief does not list is marked "added".
// Records refer to each other by ID, never by pointer: deleting from the BST can move a parcel into
// another node, so a stored pointer could end up pointing at the wrong parcel.

#include <string>

// ---------------------------------------------------------------------------
// Parcel - registered by Task 1, organised by Task 5
// ---------------------------------------------------------------------------

enum class ParcelType {          // Task 5 calls this "Parcel Category"
    Standard,
    Large,
    Fragile,
    Document,
    SpecialHandling
};

enum class ParcelStatus {
    Received,
    Pending,
    Processing,
    ReadyForCollection,
    OutForDelivery,
    Collected,
    Delivered,                   // added: the integrated workflow ends with parcels delivered
    Returned,
    OnHold,                      // added: Task 1 can hold a parcel for investigation
    Missing,
    Cancelled
};

enum class ServiceType {
    None,                        // added: parcel not sorted yet
    RecipientCollection,
    CampusDelivery,
    SpecialHandling,
    ReturnToSender,
    Redelivery
};

struct Parcel {
    std::string  parcelId;                          // unique, e.g. "P0001"; the BST key
    std::string  recipientId;                       // student or staff ID
    std::string  recipientName;
    std::string  deliveryCompany;
    ParcelType   type        = ParcelType::Standard;
    ServiceType  serviceType = ServiceType::None;   // set when Task 1 sorts the parcel
    std::string  arrivalDateTime;
    ParcelStatus status      = ParcelStatus::Received;
};

// ---------------------------------------------------------------------------
// ServiceRequest - Task 2
// ---------------------------------------------------------------------------

enum class RequestStatus {
    Pending,
    Processing,
    Completed,
    Cancelled,
    Returned
};

struct ServiceRequest {
    std::string   requestId;                        // unique, e.g. "R0001"
    std::string   parcelId;                         // must belong to a registered parcel
    std::string   recipientId;
    std::string   recipientName;
    ServiceType   serviceType = ServiceType::RecipientCollection;
    std::string   requestDateTime;
    RequestStatus status      = RequestStatus::Pending;
    int           sequenceNo  = 0;                  // added: creation order, breaks priority ties
};

// ---------------------------------------------------------------------------
// Resource - Task 3
// ---------------------------------------------------------------------------

enum class ResourceType {
    CollectionCounter,
    DeliveryDesk,
    SpecialHandlingCounter,
    DeliveryPersonnel
};

enum class ResourceStatus {
    Available,
    Occupied,
    Unavailable,
    Maintenance
};

struct Resource {
    std::string    resourceId;                      // unique, e.g. "CC01"
    ResourceType   type     = ResourceType::CollectionCounter;
    std::string    location;                        // empty when not applicable (delivery personnel)
    ResourceStatus status   = ResourceStatus::Available;
    std::string    currentParcelId;                 // empty when free
    std::string    currentRequestId;                // empty when free
    int            transactionsHandled = 0;
};

// ---------------------------------------------------------------------------
// Activity - Task 4
// ---------------------------------------------------------------------------

enum class ActivityType {
    ParcelRegistration,
    ParcelUpdate,
    ServiceRequestCreation,
    ResourceAssignment,
    StatusChange,
    CollectionConfirmation,
    Cancellation,
    ReturnProcessing
};

struct Activity {
    int          activityId = 0;                    // goes up by one per activity, so it gives the order
    std::string  parcelId;
    std::string  requestId;                         // empty when not applicable
    std::string  resourceId;                        // added: lets undo free the right resource
    ActivityType type       = ActivityType::StatusChange;
    std::string  dateTime;
    ParcelStatus prevStatus = ParcelStatus::Received;
    ParcelStatus newStatus  = ParcelStatus::Received;
    std::string  staffId;                           // empty when not applicable
    bool         reversed   = false;                // added: an undone activity cannot be undone again
};

// ---------------------------------------------------------------------------
// Display names - kept in one place so every module prints the brief's wording
// ---------------------------------------------------------------------------

inline const char* toString(ParcelType value)
{
    switch (value) {
    case ParcelType::Standard:        return "Standard Parcel";
    case ParcelType::Large:           return "Large Parcel";
    case ParcelType::Fragile:         return "Fragile Parcel";
    case ParcelType::Document:        return "Document";
    case ParcelType::SpecialHandling: return "Special-Handling Parcel";
    }
    return "Unknown";
}

inline const char* toString(ParcelStatus value)
{
    switch (value) {
    case ParcelStatus::Received:           return "Received";
    case ParcelStatus::Pending:            return "Pending";
    case ParcelStatus::Processing:         return "Processing";
    case ParcelStatus::ReadyForCollection: return "Ready for Collection";
    case ParcelStatus::OutForDelivery:     return "Out for Delivery";
    case ParcelStatus::Collected:          return "Collected";
    case ParcelStatus::Delivered:          return "Delivered";
    case ParcelStatus::Returned:           return "Returned";
    case ParcelStatus::OnHold:             return "On Hold";
    case ParcelStatus::Missing:            return "Missing";
    case ParcelStatus::Cancelled:          return "Cancelled";
    }
    return "Unknown";
}

inline const char* toString(ServiceType value)
{
    switch (value) {
    case ServiceType::None:                return "Not assigned";
    case ServiceType::RecipientCollection: return "Recipient Collection";
    case ServiceType::CampusDelivery:      return "Campus Delivery";
    case ServiceType::SpecialHandling:     return "Special Handling";
    case ServiceType::ReturnToSender:      return "Return to Sender";
    case ServiceType::Redelivery:          return "Re-delivery";
    }
    return "Unknown";
}

inline const char* toString(RequestStatus value)
{
    switch (value) {
    case RequestStatus::Pending:    return "Pending";
    case RequestStatus::Processing: return "Processing";
    case RequestStatus::Completed:  return "Completed";
    case RequestStatus::Cancelled:  return "Cancelled";
    case RequestStatus::Returned:   return "Returned";
    }
    return "Unknown";
}

inline const char* toString(ResourceType value)
{
    switch (value) {
    case ResourceType::CollectionCounter:      return "Collection Counter";
    case ResourceType::DeliveryDesk:           return "Delivery Desk";
    case ResourceType::SpecialHandlingCounter: return "Special-Handling Counter";
    case ResourceType::DeliveryPersonnel:      return "Delivery Personnel";
    }
    return "Unknown";
}

inline const char* toString(ResourceStatus value)
{
    switch (value) {
    case ResourceStatus::Available:   return "Available";
    case ResourceStatus::Occupied:    return "Occupied";
    case ResourceStatus::Unavailable: return "Unavailable";
    case ResourceStatus::Maintenance: return "Maintenance";
    }
    return "Unknown";
}

inline const char* toString(ActivityType value)
{
    switch (value) {
    case ActivityType::ParcelRegistration:     return "Parcel Registration";
    case ActivityType::ParcelUpdate:           return "Parcel Update";
    case ActivityType::ServiceRequestCreation: return "Service Request Creation";
    case ActivityType::ResourceAssignment:     return "Resource Assignment";
    case ActivityType::StatusChange:           return "Status Change";
    case ActivityType::CollectionConfirmation: return "Collection Confirmation";
    case ActivityType::Cancellation:           return "Cancellation";
    case ActivityType::ReturnProcessing:       return "Return Processing";
    }
    return "Unknown";
}
