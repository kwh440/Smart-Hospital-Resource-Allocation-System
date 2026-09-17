#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "billing.h"
#include "file_manager.h"

void displayMenu() {
    printf("\n=======================================================\n");
    printf("     SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM     \n");
    printf("=======================================================\n");
    printf(" 1. Register Patient\n");
    printf(" 2. Display Patients (Registration Order)\n");
    printf(" 3. Display Priority Triage Queue (Emergency Priority)\n");
    printf(" 4. Search Patient Record\n");
    printf(" 5. Update Patient Record\n");
    printf(" 6. Display Specialties\n");
    printf(" 7. Display Wards\n");
    printf(" 8. Display Beds & Occupancy Metrics\n");
    printf(" 9. Allocate Bed (With Auto-Recommendation)\n");
    printf("10. Discharge Patient & Release Bed\n");
    printf("11. Calculate Basic Bill\n");
    printf("12. Exit\n");
    printf("=======================================================\n");
    printf("Enter your choice (1-12): ");
}

int main() {
    initDirectoriesAndFiles();
    initWardsAndSpecialties();
    initBeds();

    loadBedStatus();
    loadPatientRecords();

    int choice = 0;

    while (1) {
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("\n[Validation Error] Invalid input! Please enter a number between 1 and 12.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayPatients();
                break;
            case 3:
                displayPriorityTriageQueue();
                break;
            case 4:
                searchPatient();
                break;
            case 5:
                updatePatientRecord();
                break;
            case 6:
                displaySpecialties();
                break;
            case 7:
                displayWards();
                break;
            case 8:
                displayBedOccupancyMetrics();
                break;
            case 9:
                allocateBed();
                break;
            case 10:
                releaseBed();
                break;
            case 11:
                processBillCalculation();
                break;
            case 12:
                printf("\nSaving data records to disk before exiting...\n");
                savePatientRecords();
                saveBedStatus();
                printf("Data saved successfully. Thank you for using Smart Hospital System. Goodbye!\n");
                return 0;
            default:
                printf("\n[Error] Invalid choice (%d). Please select an option between 1 and 12.\n", choice);
                break;
        }
    }

    return 0;
}
