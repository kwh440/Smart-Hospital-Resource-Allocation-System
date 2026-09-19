/*
 * Smart Hospital & Resource Allocation System
 *
 * File: bed.c
 * Purpose: Bed occupancy matrix management, AI ward recommendation logic,
 *          bed allocation, discharge/release operations, and visual dashboard displays.
 */

#include <stdio.h>
#include <string.h>
#include "bed.h"
#include "ward.h"
#include "patient.h"
#include "ui_effects.h"

/* ============================================================
   GLOBAL DATA STORAGE
   ============================================================ */

/* Global array storing occupancy state for all 45 hospital beds */
Bed beds[TOTAL_BEDS_IN_HOSPITAL];

/* ============================================================
   INITIALIZATION & MATRIX RENDERING
   ============================================================ */

/*
 * Function: initBeds
 * Purpose : Populates initial hospital bed structures across 4 wards:
 *           Ward 1 (General Ward)   : Beds 101-120 (20 Beds)
 *           Ward 2 (Paediatric Ward): Beds 201-210 (10 Beds)
 *           Ward 3 (Surgical Ward)  : Beds 301-310 (10 Beds)
 *           Ward 4 (ICU)            : Beds 401-405 (5 Beds)
 * Input   : None
 * Returns : None
 */
void initBeds() {
    int bIndex = 0;

    /* Ward 1: General Ward (Beds 101 to 120) */
    for (int i = 1; i <= 20; i++) {
        beds[bIndex].bedID = 100 + i;
        beds[bIndex].wardID = 1;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    /* Ward 2: Paediatric Ward (Beds 201 to 210) */
    for (int i = 1; i <= 10; i++) {
        beds[bIndex].bedID = 200 + i;
        beds[bIndex].wardID = 2;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    /* Ward 3: Surgical Ward (Beds 301 to 310) */
    for (int i = 1; i <= 10; i++) {
        beds[bIndex].bedID = 300 + i;
        beds[bIndex].wardID = 3;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    /* Ward 4: ICU (Beds 401 to 405) */
    for (int i = 1; i <= 5; i++) {
        beds[bIndex].bedID = 400 + i;
        beds[bIndex].wardID = 4;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }
}

/*
 * Function: displayBedMatrix
 * Purpose : Renders a 2D grid matrix of bed availability (0 = Available [GREEN], 1 = Occupied [RED]).
 * Input   : None
 * Returns : None
 */
void displayBedMatrix() {
    displayColorLegend();

    printf("\n%s                      BED OCCUPANCY MATRIX (0 = Available, 1 = Occupied)%s\n\n", COLOR_CYAN, COLOR_RESET);
    printf("                 Bed\n");
    printf("        01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20\n");
    printf("       +------------------------------------------------------------------------------\n");

    for (int w = 1; w <= 4; w++) {
        printf("Ward %d |", w);
        int wardMaxBeds = (w == 1) ? 20 : (w == 2 || w == 3) ? 10 : 5;

        for (int b = 1; b <= wardMaxBeds; b++) {
            int bedID = (w * 100) + b;
            int status = 0;

            for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
                if (beds[i].bedID == bedID) {
                    status = beds[i].status;
                    break;
                }
            }
            if (status == 1) {
                printf(" %s1%s  ", COLOR_RED, COLOR_RESET); /* Occupied bed */
            } else {
                printf(" %s0%s  ", COLOR_GREEN, COLOR_RESET); /* Available bed */
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------------------------------------------------\n");
}

/*
 * Function: displayBoxedWardBedStatus
 * Purpose : Renders double-line boxed bed status panels per ward.
 * Input   : None
 * Returns : None
 */
void displayBoxedWardBedStatus() {
    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;

        int wardBeds = ward->totalBeds;
        int occupied = 0;

        char titleStr[60];
        snprintf(titleStr, sizeof(titleStr), "%s BED STATUS", ward->name);

        printf("\n%s╔══════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  %-38s  %s║%s\n", COLOR_CYAN, COLOR_RESET, titleStr, COLOR_CYAN, COLOR_RESET);
        printf("%s╠══════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║                                          ║%s\n", COLOR_CYAN, COLOR_RESET);

        for (int b = 1; b <= wardBeds; b++) {
            int bedID = (w * 100) + b;
            int status = 0;
            for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
                if (beds[i].bedID == bedID) {
                    status = beds[i].status;
                    break;
                }
            }
            if (status == 1) {
                occupied++;
                printf("%s║%s  Bed %02d  [%s OCCUPIED %s]   %sRED%s               %s║%s\n",
                       COLOR_CYAN, COLOR_RESET, b, COLOR_RED, COLOR_RESET, COLOR_RED, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
            } else {
                printf("%s║%s  Bed %02d  [%s AVAILABLE %s]  %sGREEN%s            %s║%s\n",
                       COLOR_CYAN, COLOR_RESET, b, COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
            }
        }

        float pct = (wardBeds > 0) ? ((float)occupied / (float)wardBeds) * 100.0f : 0.0f;
        char occBuf[50];
        snprintf(occBuf, sizeof(occBuf), "Occupancy: %.1f %%", pct);

        printf("%s║                                          ║%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  %-38s  %s║%s\n", COLOR_CYAN, COLOR_RESET, occBuf, COLOR_CYAN, COLOR_RESET);
        printf("%s╚══════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
    }
}

/*
 * Function: displayGraphicalBedCards
 * Purpose : Renders 5-wide horizontal graphical bed dashboard cards color-coded RED/GREEN.
 * Input   : None
 * Returns : None
 */
void displayGraphicalBedCards() {
    printf("\n%s========================================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s                     GRAPHICAL BED DASHBOARD CARDS                     %s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s========================================================================%s\n", COLOR_CYAN, COLOR_RESET);

    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;

        int wardBeds = ward->totalBeds;
        printf("\n%s[%s]%s\n", COLOR_CYAN, ward->name, COLOR_RESET);

        for (int r = 0; r < wardBeds; r += 5) {
            int rowCount = (r + 5 <= wardBeds) ? 5 : (wardBeds - r);

            for (int c = 0; c < rowCount; c++) printf("+--------+  ");
            printf("\n");

            for (int c = 0; c < rowCount; c++) printf("| BED %02d |  ", r + c + 1);
            printf("\n");

            for (int c = 0; c < rowCount; c++) {
                int bNum = r + c + 1;
                int bedID = (w * 100) + bNum;
                int status = 0;
                for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
                    if (beds[i].bedID == bedID) {
                        status = beds[i].status;
                        break;
                    }
                }
                if (status == 1) {
                    printf("| %s####%s   |  ", COLOR_RED, COLOR_RESET);
                } else {
                    printf("|        |  ");
                }
            }
            printf("\n");

            for (int c = 0; c < rowCount; c++) {
                int bNum = r + c + 1;
                int bedID = (w * 100) + bNum;
                int status = 0;
                for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
                    if (beds[i].bedID == bedID) {
                        status = beds[i].status;
                        break;
                    }
                }
                if (status == 1) {
                    printf("| %sOCCUP.%s |  ", COLOR_RED, COLOR_RESET);
                } else {
                    printf("|  %sFREE%s  |  ", COLOR_GREEN, COLOR_RESET);
                }
            }
            printf("\n");

            for (int c = 0; c < rowCount; c++) printf("+--------+  ");
            printf("\n");
        }
    }
}

/* ============================================================
   BED STATUS TABLES & SEARCH
   ============================================================ */

/*
 * Function: displayBeds
 * Purpose : Displays 4 unified ward bed status tables with bed IDs, occupancy state,
 *           assigned patient IDs, and patient names.
 * Input   : None
 * Returns : None
 */
void displayBeds() {
    displayColorLegend();
    displayBedMatrix();
    displayGraphicalBedCards();

    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;

        char title[60];
        snprintf(title, sizeof(title), "%s BED STATUS", ward->name);

        printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║  %-93s     ║%s\n", COLOR_CYAN, title, COLOR_RESET);
        printf("%s╠══════════════════════╦═════════╦════════╦══════════════╦═════════════════╦═════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s Ward Name            %s║%s Ward ID %s║%s Bed ID %s║%s Status       %s║%s Patient ID    %s  ║%s Assigned Patient Name %s  ║%s\n",
               COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_RESET, COLOR_RESET);
        printf("%s╠══════════════════════╬═════════╬════════╬══════════════╬═════════════════╬═════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);

        for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
            if (beds[i].wardID == w) {
                const char *statusStr = (beds[i].status == 1) ? "Occupied" : "Available";
                const char *statusColor = (beds[i].status == 1) ? COLOR_RED : COLOR_GREEN;

                const char *pID = beds[i].assignedPatientID;
                const char *pName = "None";

                if (beds[i].status == 1 && strcmp(pID, "None") != 0) {
                    Patient *p = findPatientByID(pID);
                    if (p != NULL) {
                        pName = p->name;
                    } else {
                        pName = "N/A";
                    }
                }

                printf("%s║%s %-20s %s║%s %-7d %s║%s %-6d %s║%s %s%-12s%s %s║%s %-15s %s║%s %-23s %s║%s\n",
                       COLOR_CYAN, COLOR_RESET,
                       ward->name,
                       COLOR_CYAN, COLOR_RESET,
                       w,
                       COLOR_CYAN, COLOR_RESET,
                       beds[i].bedID,
                       COLOR_CYAN, COLOR_RESET,
                       statusColor, statusStr, COLOR_RESET,
                       COLOR_CYAN, COLOR_RESET,
                       pID,
                       COLOR_CYAN, COLOR_RESET,
                       pName,
                       COLOR_CYAN, COLOR_RESET);
            }
        }
        printf("%s╚══════════════════════╩═════════╩════════╩══════════════╩═════════════════╩═════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
    }
}

/*
 * Function: findAvailableBed
 * Purpose : Searches for the first unassigned bed (status == 0) in the specified ward.
 * Input   : wardID - Target ward ID (1 to 4).
 * Returns : Bed ID if available, or -1 if no beds are free.
 */
int findAvailableBed(int wardID) {
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].wardID == wardID && beds[i].status == 0) {
            return beds[i].bedID; /* Return first available bed ID */
        }
    }
    return -1; /* All beds in ward occupied */
}

/* ============================================================
   AI RECOMMENDATION & BED ALLOCATION
   ============================================================ */

/*
 * Function: recommendWard
 * Purpose : Evaluates patient emergency status and age to recommend optimal ward:
 *           - Level 3 Critical -> Ward 4 (ICU)
 *           - Age < 12         -> Ward 2 (Paediatric Ward)
 *           - Level 2 Urgent   -> Ward 3 (Surgical Ward)
 *           - Level 1 Normal   -> Ward 1 (General Ward)
 * Input   : emergencyStatus - Urgency level (1, 2, 3).
 *           age             - Patient age in years.
 * Returns : Recommended Ward ID (1 to 4).
 */
int recommendWard(int emergencyStatus, int age) {
    if (emergencyStatus == 3) {
        return 4; /* Critical emergency -> ICU */
    }
    if (age < 12) {
        return 2; /* Child -> Paediatric Ward */
    }
    if (emergencyStatus == 2) {
        return 3; /* Urgent medical condition -> Surgical Ward */
    }
    return 1;     /* Normal condition -> General Ward */
}

/*
 * Function: displayBedOccupancyMetrics
 * Purpose : Prints ward bed utilization summary report including total, occupied,
 *           and available bed counts along with occupancy percentages.
 * Input   : None
 * Returns : None
 */
void displayBedOccupancyMetrics() {
    displayBeds();

    printf("\n=========================================================================\n");
    printf("                  WARD BED OCCUPANCY METRICS SUMMARY                     \n");
    printf("=========================================================================\n");
    printf("%-8s %-20s %-12s %-12s %-12s %-12s\n",
           "Ward ID", "Ward Name", "Total Beds", "Occupied", "Available", "Occupancy %");
    printf("-------------------------------------------------------------------------\n");

    int grandTotal = 0, grandOccupied = 0, grandAvailable = 0;

    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;

        int occupiedCount = 0;
        for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
            if (beds[i].wardID == w && beds[i].status == 1) {
                occupiedCount++;
            }
        }

        int availCount = ward->totalBeds - occupiedCount;
        float occPct = (ward->totalBeds > 0) ? ((float)occupiedCount / (float)ward->totalBeds) * 100.0f : 0.0f;

        grandTotal += ward->totalBeds;
        grandOccupied += occupiedCount;
        grandAvailable += availCount;

        printf("%-8d %-20s %-12d %-12d %-12d %-11.1f%%\n",
               ward->wardID, ward->name, ward->totalBeds, occupiedCount, availCount, occPct);
    }

    float grandPct = (grandTotal > 0) ? ((float)grandOccupied / (float)grandTotal) * 100.0f : 0.0f;
    printf("-------------------------------------------------------------------------\n");
    printf("%-8s %-20s %-12d %-12d %-12d %-11.1f%%\n",
           "TOTAL", "All Wards Combined", grandTotal, grandOccupied, grandAvailable, grandPct);
    printf("=========================================================================\n");
}

/*
 * Function: allocateBed
 * Purpose : Interactively allocates an available bed to a registered patient based on
 *           AI ward recommendation or user-selected ward.
 * Input   : None
 * Returns : None
 */
void allocateBed() {
    if (patientCount == 0) {
        printf("\n[Error] No registered patients found. Please register a patient first.\n");
        return;
    }

    char patientID[15];
    printf("\nEnter Patient ID to allocate a bed (e.g., PAT-1001): ");
    scanf("%14s", patientID);

    Patient *p = findPatientByID(patientID);
    if (p == NULL) {
        printf("[Error] Patient with ID '%s' was not found in records.\n", patientID);
        return;
    }

    if (p->bedID != 0) {
        printf("[Notice] Patient %s is already allocated Bed #%d in Ward #%d.\n", p->id, p->bedID, p->wardID);
        return;
    }

    /* Compute AI ward recommendation */
    int recommendedWardID = recommendWard(p->emergencyStatus, p->age);
    Ward *recWard = getWardByID(recommendedWardID);

    printf("\n%s[AI Recommendation]%s Based on age (%s%d years%s) & status (%s%s%s), recommended ward is: %sWard #%d (%s)%s\n",
           COLOR_CYAN, COLOR_RESET,
           COLOR_YELLOW, p->age, COLOR_RESET,
           (p->emergencyStatus == 3) ? COLOR_RED : (p->emergencyStatus == 2) ? COLOR_YELLOW : COLOR_GREEN,
           (p->emergencyStatus == 3) ? "Critical" : (p->emergencyStatus == 2) ? "Urgent" : "Normal",
           COLOR_RESET,
           COLOR_GREEN, recommendedWardID, recWard ? recWard->name : "General", COLOR_RESET);

    displayWards();
    int wardID;
    printf("Select Target Ward ID for admission (1-4) [Press 0 for recommended Ward #%d]: ", recommendedWardID);
    if (scanf("%d", &wardID) != 1) {
        printf("[Error] Invalid Ward ID entered. Allocation cancelled.\n");
        return;
    }

    if (wardID == 0) {
        wardID = recommendedWardID;
    } else if (wardID < 1 || wardID > 4) {
        printf("[Error] Invalid Ward ID selection.\n");
        return;
    }

    showBedScanningAnimation(wardID);
    int bedID = findAvailableBed(wardID);
    if (bedID == -1) {
        printf("[Allocation Error] Sorry, all beds in Ward #%d are currently full!\n", wardID);
        return;
    }

    /* Mark bed occupied and associate patient ID */
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].bedID == bedID) {
            beds[i].status = 1;
            strcpy(beds[i].assignedPatientID, p->id);
            p->wardID = wardID;
            p->bedID = bedID;

            Ward *w = getWardByID(wardID);
            if (w != NULL && w->availableBeds > 0) {
                w->availableBeds--;
            }

            showBedAllocationVisual(bedID, p->id, p->name);
            printf("\n[Success] Bed #%d in Ward #%d successfully allocated to Patient '%s' (%s)!\n",
                   bedID, wardID, p->name, p->id);
            return;
        }
    }
}

/* ============================================================
   PATIENT DISCHARGE & BED RELEASE
   ============================================================ */

/*
 * Function: releaseBed
 * Purpose : Discharges an admitted patient, releases their allocated bed (sets status = 0),
 *           resets assigned patient ID to "None", and restores available bed counter.
 * Input   : None
 * Returns : None
 */
void releaseBed() {
    if (patientCount == 0) {
        printf("\n[Error] No registered patients found in system.\n");
        return;
    }

    char patientID[15];
    printf("\nEnter Patient ID to discharge and release bed (e.g. PAT-1001): ");
    scanf("%14s", patientID);

    Patient *p = findPatientByID(patientID);
    if (p == NULL) {
        printf("[Error] Patient with ID '%s' was not found.\n", patientID);
        return;
    }

    if (p->bedID == 0) {
        printf("[Notice] Patient '%s' (%s) is not currently admitted to any bed.\n", p->name, p->id);
        return;
    }

    int oldBedID = p->bedID;
    int oldWardID = p->wardID;

    /* Find allocated bed and restore to free state */
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].bedID == oldBedID) {
            beds[i].status = 0; /* Set bed status to Available */
            strcpy(beds[i].assignedPatientID, "None");

            Ward *w = getWardByID(oldWardID);
            if (w != NULL) {
                w->availableBeds++; /* Increment available bed counter */
            }

            p->wardID = 0; /* Reset patient ward to OPD status */
            p->bedID = 0;  /* Reset patient bed to OPD status */

            printf("\n[Success] Patient '%s' (%s) successfully discharged! Bed #%d in Ward #%d is now Available.\n",
                   p->name, p->id, oldBedID, oldWardID);
            return;
        }
    }
}
