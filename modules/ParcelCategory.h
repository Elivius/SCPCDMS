#pragma once
// Task 5 - Parcel Category Organisation
// Uses: ParcelBST (ds/ParcelBST.h) keyed on Parcel ID - the system's one parcel registry.
// Covers: create/maintain the tree, insert, search, display by traversal,
//         update/remove while keeping BST order, exceptional conditions.
// Works with: every module that needs to find or update a parcel.

#include <string>
#include "../Models.h"
#include "../ds/ParcelBST.h"

class ParcelCategory {
private:
    ParcelBST tree_;

public:
    // ---- API for other modules (no console input; they return false / nullptr and let the caller report) ----
    bool          registerParcel(const Parcel& parcel);          // validates, rejects duplicate ID
    const Parcel* find(const std::string& parcelId) const;
    bool          updateParcel(const Parcel& parcel);            // replaces all fields except the ID
    bool          setStatus(const std::string& parcelId, ParcelStatus status);
    bool          setServiceType(const std::string& parcelId, ServiceType type);
    bool          removeParcel(const std::string& parcelId);
    int           count() const { return tree_.size(); }

    // ---- Validation (return an empty string when valid, otherwise the reason) ----
    static std::string checkParcelId(const std::string& id);
    static std::string checkParcel(const Parcel& parcel);

    // ---- Console menu for Task 5 ----
    void run();

private:
    void insertMenu();
    void searchMenu() const;
    void displayMenu() const;
    void updateMenu();
    void deleteMenu();
    void filterByMenu() const;

    void showTree() const;           // pre-order, indented by depth
    void showSorted() const;         // in-order, table sorted by Parcel ID
};
