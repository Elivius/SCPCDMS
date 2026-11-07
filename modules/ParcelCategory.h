#pragma once
// Task 5 - Parcel Category Organisation (BST on Parcel ID)

#include <string>
#include "../Models.h"
#include "../ds/ParcelBST.h"

class ParcelCategory {
private:
    ParcelBST tree_;

public:
    // For other modules
    bool          registerParcel(const Parcel& parcel);
    const Parcel* find(const std::string& parcelId) const;
    bool          updateParcel(const Parcel& parcel);
    bool          setStatus(const std::string& parcelId, ParcelStatus status);
    bool          setServiceType(const std::string& parcelId, ServiceType type);
    bool          removeParcel(const std::string& parcelId);
    int           count() const { return tree_.size(); }
    void          displayAll() const;                            // sorted by Parcel ID

    // visit(const Parcel&) in ascending Parcel ID order
    template <typename Visitor>
    void forEach(Visitor visit) const { tree_.inOrder(visit); }

    // Empty string means valid
    static std::string checkParcelId(const std::string& id);
    static std::string checkParcel(const Parcel& parcel);

    // Console menu
    void run();

private:
    void insertMenu();
    void searchMenu() const;
    void displayMenu() const;
    void updateMenu();
    void deleteMenu();
    void filterByMenu() const;

    void showTree() const;
    void showSorted() const;
};
