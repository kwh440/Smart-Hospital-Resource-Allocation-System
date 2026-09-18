#include <stdio.h>
#include <string.h>
#include "bed.h"
#include "ward.h"
#include "patient.h"
#include "ui_effects.h"

Bed beds[TOTAL_BEDS_IN_HOSPITAL];

void initBeds() {
    int bIndex = 0;

    for (int i = 1; i <= 20; i++) {
        beds[bIndex].bedID = 100 + i;
        beds[bIndex].wardID = 1;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    for (int i = 1; i <= 10; i++) {
        beds[bIndex].bedID = 200 + i;
        beds[bIndex].wardID = 2;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    for (int i = 1; i <= 10; i++) {
        beds[bIndex].bedID = 300 + i;
        beds[bIndex].wardID = 3;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }

    for (int i = 1; i <= 5; i++) {
        beds[bIndex].bedID = 400 + i;
        beds[bIndex].wardID = 4;
        beds[bIndex].status = 0;
        strcpy(beds[bIndex].assignedPatientID, "None");
        bIndex++;
    }
}

void displayBedMatrix() {
    printf("\n                      BED OCCUPANCY MATRIX (0 = Available, 1 = Occupied)\n\n");
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
            printf(" %d  ", status);
        }
        printf("\n");
    }
    printf("--------------------------------------------------------------------------------------\n");
}

void displayBeds() {
    displayBedMatrix();

    printf("\n===================================================\n");
    printf("              HOSPITAL BED OCCUPANCY LIST          \n");
    printf("===================================================\n");
    printf("%-8s %-8s %-12s %-18s\n", "Bed ID", "Ward ID", "Status", "Assigned Patient");
    printf("---------------------------------------------------\n");
    
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        const char *statusStr = (beds[i].status == 1) ? "Occupied" : "Available";
        printf("%-8d %-8d %-12s %-18s\n",
               beds[i].bedID,
               beds[i].wardID,
               statusStr,
               beds[i].assignedPatientID);
    }
    printf("---------------------------------------------------\n");
}

int findAvailableBed(int wardID) {
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].wardID == wardID && beds[i].status == 0) {
            return beds[i].bedID;
        }
    }
    return -1;
}

int recommendWard(int emergencyStatus, int age) {
    if (emergencyStatus == 3) {
        return 4; // Critical -> ICU (Ward 4)
    }
    if (age < 12) {
        return 2; // Child -> Pediatric Ward (Ward 2)
    }
    if (emergencyStatus == 2) {
        return 3; // Urgent -> Emergency Ward (Ward 3)
    }
    return 1; // Normal -> General Ward (Ward 1)
}

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

    int recommendedWardID = recommendWard(p->emergencyStatus, p->age);
    Ward *recWard = getWardByID(recommendedWardID);
    
    printf("\n[AI Recommendation] Based on age (%d) & status (%s), recommended ward is: Ward #%d (%s)\n",
           p->age,
           (p->emergencyStatus == 3) ? "Critical" : (p->emergencyStatus == 2) ? "Urgent" : "Normal",
           recommendedWardID,
           recWard ? recWard->name : "General");

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

    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].bedID == oldBedID) {
            beds[i].status = 0; // Set bed to Available
            strcpy(beds[i].assignedPatientID, "None");

            Ward *w = getWardByID(oldWardID);
            if (w != NULL) {
                w->availableBeds++; // Restore available bed count
            }

            p->wardID = 0; // Reset patient ward ID to OPD status
            p->bedID = 0;  // Reset patient bed ID to OPD status

            printf("\n[Success] Patient '%s' (%s) successfully discharged! Bed #%d in Ward #%d is now Available.\n",
                   p->name, p->id, oldBedID, oldWardID);
            return;
        }
    }
}
