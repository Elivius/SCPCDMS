#include "ParcelCategory.h"
#include "../Utils.h"

#include <iostream>
#include <iomanip>


std::string ParcelCategory::checkParcelId(const std::string& id)
{
    // Fixed width so text order matches numeric order
    if (id.size() != 5 || (id[0] != 'P'))
        return "Parcel ID must look like P0001 (letter P + 4 digits).";
    for (size_t i = 1; i < id.size(); ++i)
        if (id[i] < '0' || id[i] > '9')
            return "Parcel ID must look like P0001 (letter P + 4 digits).";
    return "";
}

std::string ParcelCategory::checkParcel(const Parcel& p)
{
    std::string err = checkParcelId(p.parcelId);
    if (!err.empty()) return err;

    if (p.recipientId.empty() || p.recipientId.size() > 10)
        return "Recipient ID is required (1-10 letters/digits).";
    for (char c : p.recipientId)
        if (!isAlnum(c)) return "Recipient ID may only contain letters and digits.";

    if (p.recipientName.empty()) return "Recipient name is required.";
    if (p.deliveryCompany.empty()) return "Delivery company is required.";
    if (!validDateTime(p.arrivalDateTime)) return "Arrival time must be YYYY-MM-DD HH:MM.";
    return "";
}


bool ParcelCategory::registerParcel(const Parcel& parcel)
{
    if (!checkParcel(parcel).empty()) return false;
    return tree_.insert(parcel);
}

const Parcel* ParcelCategory::find(const std::string& parcelId) const
{
    return tree_.search(parcelId);
}

bool ParcelCategory::updateParcel(const Parcel& parcel)
{
    if (!checkParcel(parcel).empty()) return false;
    return tree_.update(parcel);
}

bool ParcelCategory::setStatus(const std::string& parcelId, ParcelStatus status)
{
    const Parcel* p = tree_.search(parcelId);
    if (p == nullptr) return false;
    Parcel copy = *p;
    copy.status = status;
    return tree_.update(copy);
}

bool ParcelCategory::setServiceType(const std::string& parcelId, ServiceType type)
{
    const Parcel* p = tree_.search(parcelId);
    if (p == nullptr) return false;
    Parcel copy = *p;
    copy.serviceType = type;
    return tree_.update(copy);
}

bool ParcelCategory::removeParcel(const std::string& parcelId)
{
    return tree_.remove(parcelId);
}


void ParcelCategory::run()
{
    for (;;) {
        std::cout << "\n=== Task 5 : Parcel Category Organisation (BST on Parcel ID) ===\n"
                  << " Parcels in tree: " << tree_.size() << "   Tree height: " << tree_.height() << "\n"
                  << " 1. Insert parcel\n"
                  << " 2. Search parcel\n"
                  << " 3. Display parcels (traversals)\n"
                  << " 4. Update parcel\n"
                  << " 5. Delete parcel\n"
                  << " 6. List parcels by courier / category\n"
                  << " 0. Back\n";
        switch (readChoice("Choice: ")) {
        case 0: return;
        case 1: insertMenu();   break;
        case 2: searchMenu();   break;
        case 3: displayMenu();  break;
        case 4: updateMenu();   break;
        case 5: deleteMenu();   break;
        case 6: filterByMenu(); break;
        default: std::cout << "Invalid choice. Enter a number from the menu.\n";
        }
    }
}

void ParcelCategory::insertMenu()
{
    Parcel p;
    std::string line;
    std::cout << "-- Insert parcel --\n";

    readLine("  Parcel ID (e.g. P0001): ", line);
    p.parcelId = toUpper(line);
    std::string err = checkParcelId(p.parcelId);
    if (!err.empty()) { std::cout << "Rejected: " << err << "\n"; return; }
    if (tree_.search(p.parcelId) != nullptr) {
        std::cout << "Rejected: duplicate key - parcel " << p.parcelId << " already exists.\n";
        return;
    }

    readLine("  Recipient ID: ", p.recipientId);
    readLine("  Recipient name: ", p.recipientName);
    readLine("  Delivery company: ", p.deliveryCompany);
    if (!chooseType(p.type)) { std::cout << "Rejected: invalid category.\n"; return; }
    readLine("  Arrival date/time (YYYY-MM-DD HH:MM): ", p.arrivalDateTime);
    p.serviceType = ServiceType::None;
    p.status = ParcelStatus::Received;

    err = checkParcel(p);
    if (!err.empty()) { std::cout << "Rejected: " << err << "\n"; return; }

    if (tree_.insert(p)) {
        std::cout << "Inserted:\n";
        printDetail(p);
    } else {
        std::cout << "Rejected: could not insert parcel.\n";
    }
}

void ParcelCategory::searchMenu() const
{
    std::cout << "-- Search parcel --\n";
    if (tree_.isEmpty()) { std::cout << "The tree is empty - there is nothing to search.\n"; return; }

    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    std::string err = checkParcelId(id);
    if (!err.empty()) { std::cout << "Invalid key: " << err << "\n"; return; }

    const Parcel* p = tree_.search(id);
    if (p == nullptr) std::cout << "Parcel " << id << " was not found.\n";
    else { std::cout << "Found:\n"; printDetail(*p); }
}

void ParcelCategory::showSorted() const
{
    // In-order gives ascending Parcel ID
    printHeader();
    tree_.inOrder([](const Parcel& p) { printRow(p); });
    std::cout << tree_.size() << " parcel(s).\n";
}

void ParcelCategory::showTree() const
{
    // Pre-order shows the tree shape
    tree_.preOrder([](const Parcel& p, int depth) {
        std::cout << std::string(static_cast<size_t>(depth) * 4, ' ')
                  << (depth == 0 ? "[root] " : "|-- ")
                  << p.parcelId << "  (" << toString(p.status) << ")\n";
    });
    std::cout << "Height: " << tree_.height() << ", parcels: " << tree_.size() << "\n";
}

void ParcelCategory::displayMenu() const
{
    std::cout << "-- Display --\n";
    if (tree_.isEmpty()) { std::cout << "The tree is empty - no parcels to display.\n"; return; }

    std::cout << " 1. In-order   : all parcels sorted by Parcel ID\n"
              << " 2. Pre-order  : tree structure (parent before children)\n";
    switch (readChoice("Choice: ")) {
    case 1: showSorted(); break;
    case 2: showTree();   break;
    default: std::cout << "Invalid choice.\n";
    }
}

void ParcelCategory::updateMenu()
{
    std::cout << "-- Update parcel --\n";
    if (tree_.isEmpty()) { std::cout << "The tree is empty - there is nothing to update.\n"; return; }

    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    std::string err = checkParcelId(id);
    if (!err.empty()) { std::cout << "Invalid key: " << err << "\n"; return; }

    const Parcel* found = tree_.search(id);
    if (found == nullptr) { std::cout << "Parcel " << id << " does not exist - cannot update.\n"; return; }

    Parcel p = *found;
    std::cout << "Current record:\n";
    printDetail(p);
    std::cout << " Update: 1 Status  2 Category  3 Recipient  4 Courier  0 Cancel\n"
              << " (the Parcel ID is the key and cannot be changed; delete and re-insert instead)\n";
    switch (readChoice("Choice: ")) {
    case 0: return;
    case 1: if (!chooseStatus(p.status)) { std::cout << "Invalid status. No change made.\n"; return; } break;
    case 2: if (!chooseType(p.type)) { std::cout << "Invalid category. No change made.\n"; return; } break;
    case 3:
        readLine("  New recipient ID: ", p.recipientId);
        readLine("  New recipient name: ", p.recipientName);
        break;
    case 4: readLine("  New delivery company: ", p.deliveryCompany); break;
    default: std::cout << "Invalid choice. No change made.\n"; return;
    }

    err = checkParcel(p);
    if (!err.empty()) { std::cout << "Rejected: " << err << " No change made.\n"; return; }
    if (tree_.update(p)) { std::cout << "Updated:\n"; printDetail(p); }
    else std::cout << "Update failed.\n";
}

void ParcelCategory::deleteMenu()
{
    std::cout << "-- Delete parcel --\n";
    if (tree_.isEmpty()) { std::cout << "The tree is empty - there is nothing to delete.\n"; return; }

    std::string id;
    readLine("  Parcel ID: ", id);
    id = toUpper(id);
    std::string err = checkParcelId(id);
    if (!err.empty()) { std::cout << "Invalid key: " << err << "\n"; return; }

    const Parcel* p = tree_.search(id);
    if (p == nullptr) { std::cout << "Parcel " << id << " does not exist - cannot delete.\n"; return; }

    printDetail(*p);
    std::string yn;
    readLine("  Delete this parcel? (y/n): ", yn);
    if (yn != "y" && yn != "Y") { std::cout << "Cancelled. Nothing deleted.\n"; return; }

    if (tree_.remove(id)) std::cout << "Deleted " << id << ". Tree height is now " << tree_.height() << ".\n";
    else std::cout << "Delete failed.\n";
}

void ParcelCategory::filterByMenu() const
{
    std::cout << "-- List by courier / category --\n";
    if (tree_.isEmpty()) { std::cout << "The tree is empty - no parcels to list.\n"; return; }

    std::cout << " 1. By delivery company\n 2. By category\n";
    int mode = readChoice("Choice: ");
    if (mode != 1 && mode != 2) { std::cout << "Invalid choice.\n"; return; }

    std::string company;
    ParcelType type = ParcelType::Standard;
    if (mode == 1) {
        readLine("  Delivery company: ", company);
        if (company.empty()) { std::cout << "Delivery company is required.\n"; return; }
        company = toUpper(company);
    } else if (!chooseType(type)) {
        std::cout << "Invalid category.\n";
        return;
    }

    // Not the key, so scan every node
    int matches = 0;
    printHeader();
    tree_.inOrder([&](const Parcel& p) {
        bool hit = (mode == 1) ? (toUpper(p.deliveryCompany) == company) : (p.type == type);
        if (hit) { printRow(p); ++matches; }
    });
    if (matches == 0) std::cout << "No matching parcels.\n";
    else std::cout << matches << " matching parcel(s).\n";
}

void ParcelCategory::displayAll() const
{
    if (tree_.isEmpty()) { std::cout << "No parcels registered.\n"; return; }
    showSorted();
}
