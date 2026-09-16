#include <stdio.h>
#include <string.h>
#include "bed.h"
#include "ward.h"
#include "patient.h"

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

void displayBeds() {
    printf("\n===================================================\n");
    printf("              HOSPITAL BED OCCUPANCY STATUS        \n");
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

    displayWards();
    int wardID;
    printf("Select Target Ward ID for admission (1-4): ");
    if (scanf("%d", &wardID) != 1 || wardID < 1 || wardID > 4) {
        printf("[Error] Invalid Ward ID entered. Allocation cancelled.\n");
        return;
    }

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

            printf("\n[Success] Bed #%d in Ward #%d successfully allocated to Patient '%s' (%s)!\n",
                   bedID, wardID, p->name, p->id);
            return;
        }
    }
}
