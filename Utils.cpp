#include "Utils.h"

#include <iostream>
#include <iomanip>

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
