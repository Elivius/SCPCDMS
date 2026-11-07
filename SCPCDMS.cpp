// SCPCDMS - Smart Campus Parcel Collection and Delivery Management System
// Top-level menu. Task 1 (Queue) registers parcels into Task 5 (BST).

#include <iostream>
#include "Utils.h"
#include "modules/ParcelCategory.h"
#include "modules/ParcelIntake.h"

int main()
{
    ParcelCategory parcels;
    ParcelIntake intake(parcels);
    intake.loadSampleData();

    for (;;) {
        std::cout << "\n=== SCPCDMS ===\n"
                  << " 1. Task 1 - Parcel Intake (Queue)\n"
                  << " 5. Task 5 - Parcel Category Organisation (BST)\n"
                  << " 0. Exit\n";
        switch (readChoice("Choice: ")) {
        case 0: std::cout << "Goodbye.\n"; return 0;
        case 1: intake.run();  break;
        case 5: parcels.run(); break;
        default: std::cout << "Invalid choice. Enter a number from the menu.\n";
        }
    }
}
