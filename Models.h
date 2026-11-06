#pragma once
// Shared records. Only what the parcel BST (Task 5) needs for now.

#include <string>

enum class ParcelType { Standard, Large, Fragile, Document, SpecialHandling };

enum class ParcelStatus {
    Received, Pending, Processing, ReadyForCollection, OutForDelivery,
    Collected, Delivered, Returned, OnHold, Missing, Cancelled
};

enum class ServiceType {
    None, RecipientCollection, CampusDelivery, SpecialHandling, ReturnToSender, Redelivery
};

struct Parcel {
    std::string  parcelId;          // BST key, e.g. "P0001"
    std::string  recipientId;
    std::string  recipientName;
    std::string  deliveryCompany;
    ParcelType   type        = ParcelType::Standard;
    ServiceType  serviceType = ServiceType::None;
    std::string  arrivalDateTime;
    ParcelStatus status      = ParcelStatus::Received;
};

inline const char* toString(ParcelType v)
{
    switch (v) {
    case ParcelType::Standard:        return "Standard Parcel";
    case ParcelType::Large:           return "Large Parcel";
    case ParcelType::Fragile:         return "Fragile Parcel";
    case ParcelType::Document:        return "Document";
    case ParcelType::SpecialHandling: return "Special-Handling Parcel";
    }
    return "Unknown";
}

inline const char* toString(ParcelStatus v)
{
    switch (v) {
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

inline const char* toString(ServiceType v)
{
    switch (v) {
    case ServiceType::None:                return "Not assigned";
    case ServiceType::RecipientCollection: return "Recipient Collection";
    case ServiceType::CampusDelivery:      return "Campus Delivery";
    case ServiceType::SpecialHandling:     return "Special Handling";
    case ServiceType::ReturnToSender:      return "Return to Sender";
    case ServiceType::Redelivery:          return "Re-delivery";
    }
    return "Unknown";
}
