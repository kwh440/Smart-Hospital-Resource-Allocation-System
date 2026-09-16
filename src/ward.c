#include <stdio.h>
#include <string.h>
#include "ward.h"

Ward wards[MAX_WARDS];
Specialty specialties[MAX_SPECIALTIES];

void initWardsAndSpecialties() {
    wards[0] = (Ward){1, "General Ward", 3000.0f, 20, 20};
    wards[1] = (Ward){2, "Pediatric Ward", 6000.0f, 10, 10};
    wards[2] = (Ward){3, "Emergency Ward", 12000.0f, 10, 10};
    wards[3] = (Ward){4, "ICU", 25000.0f, 5, 5};

    specialties[0] = (Specialty){1, "General Medicine", 1500.0f};
    specialties[1] = (Specialty){2, "Pediatrics", 2500.0f};
    specialties[2] = (Specialty){3, "Cardiology", 4500.0f};
    specialties[3] = (Specialty){4, "Neurology", 5000.0f};
    specialties[4] = (Specialty){5, "Orthopedics", 3500.0f};
}

void displayWards() {
    printf("\n===========================================================================\n");
    printf("                           HOSPITAL WARDS LIST                             \n");
    printf("===========================================================================\n");
    printf("%-8s %-20s %-15s %-12s %-15s\n", "Ward ID", "Ward Name", "Rate (LKR/Day)", "Total Beds", "Available Beds");
    printf("---------------------------------------------------------------------------\n");
    
    for (int i = 0; i < MAX_WARDS; i++) {
        printf("%-8d %-20s %-15.2f %-12d %-15d\n",
               wards[i].wardID,
               wards[i].name,
               wards[i].dailyRate,
               wards[i].totalBeds,
               wards[i].availableBeds);
    }
    printf("---------------------------------------------------------------------------\n");
}

void displaySpecialties() {
    printf("\n===========================================================\n");
    printf("                     DOCTOR SPECIALTIES                    \n");
    printf("===========================================================\n");
    printf("%-15s %-25s %-15s\n", "Specialty ID", "Specialty Name", "Base Fee (LKR)");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < MAX_SPECIALTIES; i++) {
        printf("%-15d %-25s %-15.2f\n",
               specialties[i].specialtyID,
               specialties[i].name,
               specialties[i].baseFee);
    }
    printf("-----------------------------------------------------------\n");
}

Ward* getWardByID(int wardID) {
    for (int i = 0; i < MAX_WARDS; i++) {
        if (wards[i].wardID == wardID) {
            return &wards[i];
        }
    }
    return NULL;
}
