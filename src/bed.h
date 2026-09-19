/*
 * Smart Hospital & Resource Allocation System
 *
 * File: bed.h
 * Purpose: Header file for hospital bed structure, bed array declaration,
 *          and bed occupancy, recommendation, allocation, and release functions.
 */

#ifndef BED_H
#define BED_H

/* ============================================================
   CONSTANTS AND CONFIGURATION
   ============================================================ */

#define TOTAL_BEDS_IN_HOSPITAL 45

/* ============================================================
   DATA STRUCTURES
   ============================================================ */

/*
 * Structure: Bed
 * Purpose  : Represents an individual hospital bed, tracking its assigned ward,
 *            occupancy status, and assigned patient ID.
 *
 * Status Codes:
 *   0 = Available (Unoccupied / Free)
 *   1 = Occupied (Reserved & Assigned to Patient)
 */
typedef struct {
    int bedID;                  /* Unique Bed ID (e.g. 101-120, 201-210, 301-310, 401-405) */
    int wardID;                 /* Ward ID where bed is located (1 to 4) */
    int status;                 /* Occupancy status: 0 = Available, 1 = Occupied */
    char assignedPatientID[15]; /* Patient ID assigned to bed, or "None" if free */
} Bed;

/* ============================================================
   GLOBAL ARRAY DECLARATIONS
   ============================================================ */

extern Bed beds[TOTAL_BEDS_IN_HOSPITAL];

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */

/* Initializes all 45 hospital beds across 4 wards with initial available state. */
void initBeds();

/* Displays 4 unified ward bed status tables with patient names. */
void displayBeds();

/* Renders 2D grid matrix representation of bed occupancy across all wards. */
void displayBedMatrix();

/*
 * Function: findAvailableBed
 * Purpose : Searches for the first available bed (status == 0) in the specified ward.
 * Input   : wardID - Target ward ID (1 to 4).
 * Returns : Available Bed ID integer, or -1 if ward is full.
 */
int findAvailableBed(int wardID);

/* Interactive bed allocation with AI recommendation. */
void allocateBed();

/*
 * Function: recommendWard
 * Purpose : Determines optimal ward recommendation based on patient age and emergency triage level.
 * Input   : emergencyStatus - Triage urgency level (1, 2, 3).
 *           age             - Patient age.
 * Returns : Recommended Ward ID (1 to 4).
 */
int recommendWard(int emergencyStatus, int age);

/* Discharges patient and marks allocated bed as available. */
void releaseBed();

/* Displays comprehensive ward bed occupancy metrics and utilization percentages. */
void displayBedOccupancyMetrics();

/* Renders boxed ward status displays. */
void displayBoxedWardBedStatus();

/* Renders 5-wide horizontal graphical bed card dashboards. */
void displayGraphicalBedCards();

#endif
