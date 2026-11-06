#include "ParcelCategory.h"

#include <iostream>
#include <iomanip>
#include <limits>

namespace {


std::string trim(const std::string& s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

std::string toUpper(std::string s)
{
    for (char& c : s)
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return s;
}

bool readLine(const std::string& prompt, std::string& out)
{
    std::cout << prompt;
    if (!std::getline(std::cin, out)) {
        std::cin.clear();
        out.clear();
        return false;
    }
    out = trim(out);
    return true;
}

// Returns -1 if not a whole number
int readChoice(const std::string& prompt)
{
    std::string line;
    if (!readLine(prompt, line)) return 0;
    if (line.empty() || line.size() > 4) return -1;
    int v = 0;
    for (char c : line) {
        if (c < '0' || c > '9') return -1;
        v = v * 10 + (c - '0');
    }
    return v;
}

bool isAlnum(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool validDateTime(const std::string& s)
{
    if (s.size() != 16) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        bool sep = (i == 4 || i == 7 || i == 10 || i == 13);
        if (sep) {
            char expect = (i == 10) ? ' ' : (i == 13 ? ':' : '-');
            if (s[i] != expect) return false;
        } else if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    int month = (s[5] - '0') * 10 + (s[6] - '0');
    int day   = (s[8] - '0') * 10 + (s[9] - '0');
    int hour  = (s[11] - '0') * 10 + (s[12] - '0');
    int min   = (s[14] - '0') * 10 + (s[15] - '0');
    return month >= 1 && month <= 12 && day >= 1 && day <= 31 && hour <= 23 && min <= 59;
}

void printHeader()
{
    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(11) << "Recipient"
              << std::setw(16) << "Name"
              << std::setw(12) << "Courier"
              << std::setw(24) << "Category"
              << std::setw(22) << "Service"
              << std::setw(22) << "Status"
              << "Arrival\n"
              << std::string(130, '-') << "\n";
}

void printRow(const Parcel& p)
{
    std::cout << std::left
              << std::setw(8)  << p.parcelId
              << std::setw(11) << p.recipientId
              << std::setw(16) << p.recipientName
              << std::setw(12) << p.deliveryCompany
              << std::setw(24) << toString(p.type)
              << std::setw(22) << toString(p.serviceType)
              << std::setw(22) << toString(p.status)
              << p.arrivalDateTime << "\n";
}

void printDetail(const Parcel& p)
{
    std::cout << "  Parcel ID      : " << p.parcelId << "\n"
              << "  Recipient ID   : " << p.recipientId << "\n"
              << "  Recipient name : " << p.recipientName << "\n"
              << "  Courier        : " << p.deliveryCompany << "\n"
              << "  Category       : " << toString(p.type) << "\n"
              << "  Service type   : " << toString(p.serviceType) << "\n"
              << "  Status         : " << toString(p.status) << "\n"
              << "  Arrival        : " << p.arrivalDateTime << "\n";
}

bool chooseType(ParcelType& out)
{
    std::cout << "  1 Standard  2 Large  3 Fragile  4 Document  5 Special-Handling\n";
    switch (readChoice("  Category: ")) {
    case 1: out = ParcelType::Standard;        return true;
    case 2: out = ParcelType::Large;           return true;
    case 3: out = ParcelType::Fragile;         return true;
    case 4: out = ParcelType::Document;        return true;
    case 5: out = ParcelType::SpecialHandling; return true;
    }
    return false;
}

bool chooseStatus(ParcelStatus& out)
{
    std::cout << "  1 Received  2 Pending  3 Processing  4 Ready for Collection  5 Out for Delivery\n"
                 "  6 Collected  7 Delivered  8 Returned  9 On Hold  10 Missing  11 Cancelled\n";
    int c = readChoice("  Status: ");
    if (c < 1 || c > 11) return false;
    static const ParcelStatus all[] = {
        ParcelStatus::Received, ParcelStatus::Pending, ParcelStatus::Processing,
        ParcelStatus::ReadyForCollection, ParcelStatus::OutForDelivery, ParcelStatus::Collected,
        ParcelStatus::Delivered, ParcelStatus::Returned, ParcelStatus::OnHold,
        ParcelStatus::Missing, ParcelStatus::Cancelled };
    out = all[c - 1];
    return true;
}

}


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
