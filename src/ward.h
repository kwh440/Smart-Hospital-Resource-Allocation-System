/*
 * Smart Hospital & Resource Allocation System
 *
 * File: ward.h
 * Purpose: Header file for hospital ward structures, doctor specialty definitions,
 *          and ward/specialty lookup function declarations.
 */

#ifndef WARD_H
#define WARD_H

/* ============================================================
   CONSTANTS AND CONFIGURATION
   ============================================================ */

#define MAX_WARDS 4
#define MAX_SPECIALTIES 7

/* ============================================================
   DATA STRUCTURES
   ============================================================ */

/*
 * Structure: Ward
 * Purpose  : Encapsulates hospital ward information including capacity,
 *            pricing, and bed availability metrics.
 */
typedef struct {
    int wardID;
    char name[30];
    float dailyRate;
    int totalBeds;
    int availableBeds;
} Ward;

/*
 * Structure: Specialty
 * Purpose  : Encapsulates medical specialty lookup data including consultation
 *            pricing, average appointment duration, and daily patient intake limits.
 */
typedef struct {
    int specialtyID;
    char name[40];
    float baseFee;
    int consultationTime; /* Average consultation time in minutes */
    int dailyCap;         /* Maximum daily patient limit */
} Specialty;

/* ============================================================
   GLOBAL ARRAY DECLARATIONS
   ============================================================ */

extern Ward wards[MAX_WARDS];
extern Specialty specialties[MAX_SPECIALTIES];

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */

/* Initializes lookup tables for hospital wards and doctor specialties. */
void initWardsAndSpecialties();

/* Displays the hospital ward directory in a formatted table. */
void displayWards();

/* Displays doctor specialties directory including base fees, times, and caps. */
void displaySpecialties();

/*
 * Function: getWardByID
 * Purpose : Searches for and returns a pointer to a Ward struct by ID.
 * Input   : wardID - Target ward identification number (1 to 4).
 * Returns : Pointer to matching Ward struct, or NULL if not found.
 */
Ward* getWardByID(int wardID);

#endif
