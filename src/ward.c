/*
 * Smart Hospital & Resource Allocation System
 *
 * File: ward.c
 * Purpose: Predefined hospital ward and doctor specialty lookup data initialization,
 *          directory table display functions, and ward lookup helper utilities.
 */

#include <stdio.h>
#include <string.h>
#include "ward.h"

/* ============================================================
   GLOBAL STORAGE DEFINITIONS
   ============================================================ */

/* Ward and specialty lookup data arrays */
Ward wards[MAX_WARDS];
Specialty specialties[MAX_SPECIALTIES];

/* ============================================================
   INITIALIZATION
   ============================================================ */

/*
 * Function: initWardsAndSpecialties
 * Purpose : Populates predefined lookup tables for hospital wards and doctor
 *           specialties with configured rates, bed capacities, fees, and time limits.
 * Input   : None
 * Returns : None
 */
void initWardsAndSpecialties() {
    /* Initialize hospital ward records (Ward ID, Name, Daily Rate, Total Beds, Available Beds) */
    wards[0] = (Ward){1, "General Ward", 3000.0f, 20, 20};
    wards[1] = (Ward){2, "Paediatric Ward", 6000.0f, 10, 10};
    wards[2] = (Ward){3, "Surgical Ward", 12000.0f, 10, 10};
    wards[3] = (Ward){4, "ICU", 25000.0f, 5, 5};

    /* Initialize doctor specialty records (Specialty ID, Name, Base Fee, Avg Time, Daily Cap) */
    specialties[0] = (Specialty){1, "General Practice (OPD)", 1500.0f, 15, 30};
    specialties[1] = (Specialty){2, "Paediatrics", 2500.0f, 20, 20};
    specialties[2] = (Specialty){3, "Cardiology", 4500.0f, 30, 12};
    specialties[3] = (Specialty){4, "Neurology", 5000.0f, 30, 10};
    specialties[4] = (Specialty){5, "Orthopedics", 3500.0f, 25, 25};
    specialties[5] = (Specialty){6, "Dermatology", 3000.0f, 15, 30};
    specialties[6] = (Specialty){7, "ENT (Ear, Nose, Throat)", 3200.0f, 15, 30};
}

/* ============================================================
   DIRECTORY DISPLAY FUNCTIONS
   ============================================================ */

/*
 * Function: displayWards
 * Purpose : Prints a formatted table listing all hospital wards, their daily rates,
 *           total bed capacity, and real-time available bed count.
 * Input   : None
 * Returns : None
 */
void displayWards() {
    printf("\n===========================================================================\n");
    printf("                           HOSPITAL WARDS LIST                             \n");
    printf("===========================================================================\n");
    printf("%-8s %-30s %-15s %-12s %-15s\n", "Ward ID", "Ward Name", "Rate (LKR/Day)", "Total Beds", "Available Beds");
    printf("---------------------------------------------------------------------------\n");
    
    for (int i = 0; i < MAX_WARDS; i++) {
        printf("%-8d %-30s %-15.2f %-12d %-15d\n",
               wards[i].wardID,
               wards[i].name,
               wards[i].dailyRate,
               wards[i].totalBeds,
               wards[i].availableBeds);
    }
    printf("---------------------------------------------------------------------------\n");
}

/*
 * Function: displaySpecialties
 * Purpose : Prints a formatted directory table listing all available doctor specialties,
 *           base consultation fees, average consultation duration, and daily patient caps.
 * Input   : None
 * Returns : None
 */
void displaySpecialties() {
    printf("\n===========================================================================================\n");
    printf("                                DOCTOR SPECIALTIES DIRECTORY                               \n");
    printf("===========================================================================================\n");
    printf("%-15s %-28s %-16s %-16s %-12s\n", "Specialty ID", "Specialty Name", "Base Fee (LKR)", "Avg Time (Mins)", "Daily Cap");
    printf("-------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < MAX_SPECIALTIES; i++) {
        printf("%-15d %-28s %-16.2f %-16d %-12d\n",
               specialties[i].specialtyID,
               specialties[i].name,
               specialties[i].baseFee,
               specialties[i].consultationTime,
               specialties[i].dailyCap);
    }
    printf("-------------------------------------------------------------------------------------------\n");
}

/* ============================================================
   UTILITY AND SEARCH FUNCTIONS
   ============================================================ */

/*
 * Function: getWardByID
 * Purpose : Searches the wards array for a matching ward ID and returns a memory pointer.
 * Input   : wardID - Target ward ID (1 to 4).
 * Returns : Pointer to the target Ward structure, or NULL if not found.
 */
Ward* getWardByID(int wardID) {
    for (int i = 0; i < MAX_WARDS; i++) {
        if (wards[i].wardID == wardID) {
            return &wards[i]; /* Return pointer to matching ward record */
        }
    }
    return NULL; /* Ward ID not found */
}
