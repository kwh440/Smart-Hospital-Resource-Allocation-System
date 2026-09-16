#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "billing.h"
#include "file_manager.h"

void displayMenu() {
    printf("\n=======================================================\n");
    printf("   SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM (V1)    \n");
    printf("=======================================================\n");
    printf(" 1. Register Patient\n");
    printf(" 2. Display Patients\n");
    printf(" 3. Display Specialties\n");
    printf(" 4. Display Wards\n");
    printf(" 5. Display Beds\n");
    printf(" 6. Allocate Bed\n");
    printf(" 7. Calculate Basic Bill\n");
    printf(" 8. Exit\n");
    printf("=======================================================\n");
    printf("Enter your choice (1-8): ");
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
            printf("\n[Validation Error] Invalid input! Please enter a number between 1 and 8.\n");
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
                displaySpecialties();
                break;
            case 4:
                displayWards();
                break;
            case 5:
                displayBeds();
                break;
            case 6:
                allocateBed();
                break;
            case 7:
                processBillCalculation();
                break;
            case 8:
                printf("\nSaving data records to disk before exiting...\n");
                savePatientRecords();
                saveBedStatus();
                printf("Data saved successfully. Thank you for using Smart Hospital System. Goodbye!\n");
                return 0;
            default:
                printf("\n[Error] Invalid choice (%d). Please select an option between 1 and 8.\n", choice);
                break;
        }
    }

    return 0;
}
