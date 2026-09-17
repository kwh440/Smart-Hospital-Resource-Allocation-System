#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "billing.h"
#include "reports.h"
#include "file_manager.h"

void displayMenu() {
    printf("\n=======================================================\n");
    printf("        SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM    \n");
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
    printf("11. Generate Performance Reports & Analytics\n");
    printf("12. Calculate Basic Bill\n");
    printf("13. Exit\n");
    printf("=======================================================\n");
}

int main() {
    initDirectoriesAndFiles();
    initWardsAndSpecialties();
    initBeds();

    loadBedStatus();
    loadPatientRecords();

    while (1) {
        displayMenu();
        
        int choice = readIntBounded("Enter your choice (1-13): ", 1, 13);

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
                generatePerformanceReport();
                break;
            case 12:
                processBillCalculation();
                break;
            case 13:
                printf("\nSaving data records to disk before exiting...\n");
                savePatientRecords();
                saveBedStatus();
                printf("Data saved successfully. Thank you for using Smart Hospital System. Goodbye!\n");
                return 0;
            default:
                break;
        }
    }

    return 0;
}
