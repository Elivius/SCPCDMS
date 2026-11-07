#pragma once
// Task 1 - Parcel Intake, Sorting and Processing
// Waiting parcels are kept as IDs in a Queue (oldest first). The records
// themselves live in ParcelCategory's BST (Task 5).

#include <string>
#include "../Models.h"
#include "../ds/Queue.h"
#include "ParcelCategory.h"

class ParcelIntake {
private:
    ParcelCategory& registry_;
    Queue<std::string> waiting_;
    int nextNumber_ = 1;

    std::string newParcelId();
    static bool isFinal(ParcelStatus s);
    static bool canChange(ParcelStatus from, ParcelStatus to);

    void registerMenu();
    void searchMenu() const;
    void displayWaiting() const;
    void sortNext();
    void updateMenu();
    void reportMissing();
    void removeMenu();
    void displayClosed() const;

public:
    explicit ParcelIntake(ParcelCategory& registry) : registry_(registry) {}

    // Returns the new Parcel ID, or "" if the parcel was rejected
    std::string registerParcel(Parcel parcel);

    // Hard-coded sample parcels for testing
    void loadSampleData();

    void run();
};
