#include "ParcelIntake.h"
#include "../Utils.h"

#include <iostream>

std::string ParcelIntake::newParcelId()
{
    for (;;) {
        std::string num = std::to_string(nextNumber_++);
        std::string id = "P" + std::string(4 - num.size(), '0') + num;
        if (registry_.find(id) == nullptr) return id;
    }
}

bool ParcelIntake::isFinal(ParcelStatus s)
{
    return s == ParcelStatus::Collected || s == ParcelStatus::Delivered ||
           s == ParcelStatus::Returned  || s == ParcelStatus::Cancelled ||
           s == ParcelStatus::Missing;
}

bool ParcelIntake::canChange(ParcelStatus from, ParcelStatus to)
{
    if (isFinal(from) || from == to) return false;
    switch (to) {
    case ParcelStatus::Collected: return from == ParcelStatus::ReadyForCollection;
    case ParcelStatus::Delivered: return from == ParcelStatus::OutForDelivery;
    case ParcelStatus::Pending:   return from == ParcelStatus::OnHold;
    case ParcelStatus::Received:  return false;
    default:                      return true;
    }
}

std::string ParcelIntake::registerParcel(Parcel p)
{
    p.parcelId = newParcelId();
    p.status = ParcelStatus::Pending;
    p.serviceType = ServiceType::None;
    if (!registry_.registerParcel(p)) return "";
    waiting_.enqueue(p.parcelId);
    return p.parcelId;
}

void ParcelIntake::loadSampleData()
{
    struct Row { const char* rid; const char* name; const char* courier; ParcelType type; const char* time; };
    const Row rows[] = {
        { "TP001", "Alice Tan",  "DHL",     ParcelType::Standard,        "2026-11-06 08:15" },
        { "TP002", "Bob Lim",    "J&T",     ParcelType::Large,           "2026-11-06 08:40" },
        { "TP003", "Chloe Wong", "PosLaju", ParcelType::Fragile,         "2026-11-06 09:05" },
        { "TP004", "Daniel Lee", "DHL",     ParcelType::Document,        "2026-11-06 09:30" },
        { "TP005", "Eva Chong",  "Ninja",   ParcelType::SpecialHandling, "2026-11-06 10:00" },
        { "TP006", "Farid Ali",  "J&T",     ParcelType::Standard,        "2026-11-06 10:20" },
    };
    for (const Row& r : rows) {
        Parcel p;
        p.recipientId = r.rid;
        p.recipientName = r.name;
        p.deliveryCompany = r.courier;
        p.type = r.type;
        p.arrivalDateTime = r.time;
        registerParcel(p);
    }
}

void ParcelIntake::run()
{
    for (;;) {
        std::cout << "\n=== Task 1 : Parcel Intake, Sorting and Processing (Queue) ===\n"
                  << " Waiting to be sorted: " << waiting_.size() << "\n"
                  << " 1. Register parcel\n"
                  << " 2. Display waiting parcels\n"
                  << " 3. Sort next parcel\n"
                  << " 4. Search parcel\n"
                  << " 5. Update parcel status\n"
                  << " 6. Report parcel missing\n"
                  << " 7. Remove closed parcel record\n"
                  << " 8. Display closed parcels\n"
                  << " 9. Display all parcels\n"
                  << " 0. Back\n";
        switch (readChoice("Choice: ")) {
        case 0: return;
        case 1: registerMenu();   break;
        case 2: displayWaiting(); break;
        case 3: sortNext();       break;
        case 4: searchMenu();     break;
        case 5: updateMenu();     break;
        case 6: reportMissing();  break;
        case 7: removeMenu();     break;
        case 8: displayClosed();  break;
        case 9: registry_.displayAll(); break;
        default: std::cout << "Invalid choice. Enter a number from the menu.\n";
        }
    }
}

void ParcelIntake::registerMenu()
{
    std::cout << "-- Register parcel --\n";
    Parcel p;
    readLine("  Recipient ID: ", p.recipientId);
    readLine("  Recipient name: ", p.recipientName);
    readLine("  Delivery company: ", p.deliveryCompany);
    if (!chooseType(p.type)) { std::cout << "Rejected: invalid parcel type.\n"; return; }
    readLine("  Arrival date/time (YYYY-MM-DD HH:MM): ", p.arrivalDateTime);

    // The real ID is assigned after validation, so check with a placeholder
    Parcel check = p;
    check.parcelId = "P0000";
    std::string err = ParcelCategory::checkParcel(check);
    if (!err.empty()) { std::cout << "Rejected: " << err << "\n"; return; }

    std::string id = registerParcel(p);
    if (id.empty()) { std::cout << "Rejected: could not register parcel.\n"; return; }
    std::cout << "Registered " << id << ". Status: Pending, queue size " << waiting_.size() << ".\n";
}

void ParcelIntake::displayWaiting() const
{
    std::cout << "-- Waiting parcels (oldest first) --\n";
    if (waiting_.isEmpty()) { std::cout << "No parcels waiting to be processed.\n"; return; }
    printHeader();
    int shown = 0;
    waiting_.forEach([&](const std::string& id) {
        const Parcel* p = registry_.find(id);
        if (p != nullptr && p->status == ParcelStatus::Pending) { printRow(*p); ++shown; }
    });
    std::cout << shown << " parcel(s) waiting.\n";
}

void ParcelIntake::sortNext()
{
    std::cout << "-- Sort next parcel --\n";
    // Parcels cancelled, missing or removed while waiting are skipped
    std::string id;
    const Parcel* found = nullptr;
    while (waiting_.dequeue(id)) {
        const Parcel* p = registry_.find(id);
        if (p != nullptr && p->status == ParcelStatus::Pending) { found = p; break; }
    }
    if (found == nullptr) { std::cout << "No parcels waiting to be processed.\n"; return; }

    Parcel p = *found;
    std::cout << "Selected parcel:\n";
    printDetail(p);

    std::cout << " Next action: 1 Default for type  2 Recipient collection  3 Campus delivery\n"
                 "              4 Special handling  5 Return to sender  6 Hold for investigation\n";
    int choice = readChoice("Choice: ");
    if (choice < 1 || choice > 6) {
        std::cout << "Invalid choice, using the default for this type.\n";
        choice = 1;
    }
    if (choice == 1) {
        switch (p.type) {
        case ParcelType::Standard:
        case ParcelType::Document:        choice = 2; break;
        case ParcelType::Large:           choice = 3; break;
        case ParcelType::Fragile:
        case ParcelType::SpecialHandling: choice = 4; break;
        }
    }
    switch (choice) {
    case 2: p.serviceType = ServiceType::RecipientCollection; p.status = ParcelStatus::ReadyForCollection; break;
    case 3: p.serviceType = ServiceType::CampusDelivery;      p.status = ParcelStatus::OutForDelivery;     break;
    case 4: p.serviceType = ServiceType::SpecialHandling;     p.status = ParcelStatus::Processing;         break;
    case 5: p.serviceType = ServiceType::ReturnToSender;      p.status = ParcelStatus::Returned;           break;
    case 6: p.serviceType = ServiceType::None;                p.status = ParcelStatus::OnHold;             break;
    }
    registry_.updateParcel(p);
    std::cout << "Parcel " << p.parcelId << " sorted: " << toString(p.serviceType)
              << ", status " << toString(p.status) << ".\n";
}

void ParcelIntake::searchMenu() const
{
    std::cout << "-- Search parcel --\n";
    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    std::string err = ParcelCategory::checkParcelId(id);
    if (!err.empty()) { std::cout << "Invalid ID: " << err << "\n"; return; }
    const Parcel* p = registry_.find(id);
    if (p == nullptr) std::cout << "Parcel " << id << " was not found.\n";
    else printDetail(*p);
}

void ParcelIntake::updateMenu()
{
    std::cout << "-- Update parcel status --\n";
    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    std::string err = ParcelCategory::checkParcelId(id);
    if (!err.empty()) { std::cout << "Invalid ID: " << err << "\n"; return; }
    const Parcel* found = registry_.find(id);
    if (found == nullptr) { std::cout << "Parcel " << id << " was not found.\n"; return; }

    std::cout << "Current status: " << toString(found->status) << "\n";
    if (isFinal(found->status)) {
        std::cout << "This parcel is already completed. No change allowed.\n";
        return;
    }
    ParcelStatus next;
    if (!chooseStatus(next)) { std::cout << "Invalid status. No change made.\n"; return; }
    if (!canChange(found->status, next)) {
        std::cout << "Cannot change " << toString(found->status) << " to " << toString(next) << ".\n";
        return;
    }
    registry_.setStatus(id, next);
    if (next == ParcelStatus::Pending) waiting_.enqueue(id);      // back in line after an investigation
    std::cout << "Parcel " << id << " is now " << toString(next) << ".\n";
}

void ParcelIntake::reportMissing()
{
    std::cout << "-- Report parcel missing --\n";
    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    const Parcel* p = registry_.find(id);
    if (p == nullptr) { std::cout << "Parcel " << id << " was not found.\n"; return; }
    if (isFinal(p->status)) {
        std::cout << "Parcel " << id << " is already " << toString(p->status) << ". Cannot report missing.\n";
        return;
    }
    registry_.setStatus(id, ParcelStatus::Missing);
    std::cout << "Parcel " << id << " marked Missing.\n";
}

void ParcelIntake::removeMenu()
{
    std::cout << "-- Remove parcel record --\n";
    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    const Parcel* p = registry_.find(id);
    if (p == nullptr) { std::cout << "Parcel " << id << " was not found.\n"; return; }
    if (!isFinal(p->status)) {
        std::cout << "Only closed parcels can be removed. Current status: " << toString(p->status) << ".\n";
        return;
    }
    registry_.removeParcel(id);
    std::cout << "Parcel " << id << " removed.\n";
}

void ParcelIntake::displayClosed() const
{
    std::cout << "-- Closed parcels (collected, delivered, returned, cancelled, missing) --\n";
    printHeader();
    int shown = 0;
    registry_.forEach([&](const Parcel& p) {
        if (isFinal(p.status)) { printRow(p); ++shown; }
    });
    if (shown == 0) std::cout << "No closed parcels.\n";
}
