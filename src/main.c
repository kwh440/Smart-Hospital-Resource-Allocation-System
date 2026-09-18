#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "billing.h"
#include "reports.h"
#include "file_manager.h"
#include "ui_effects.h"

void displayMenu() {
    printf("%s╔════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                MAIN MENU - SMART HOSPITAL          ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  1. Register Patient                               %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  2. Display Patients (Registration Order)          %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  3. Display Priority Triage Queue                  %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  4. Search Patient Record                          %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  5. Update Patient Record                          %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  6. Display Specialties                            %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  7. Display Wards                                  %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  8. Display Beds & Occupancy Metrics               %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  9. Allocate Bed (With Auto-Recommendation)        %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 10. Discharge Patient & Release Bed                %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 11. Generate Performance Reports & Analytics       %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 12. Calculate Basic Bill                           %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 13. Exit                                           %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
}

int main() {
    enableVTMode();
    displayStartupScreen();

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
                pauseConsole();
                break;
            case 2:
                displayPatients();
                pauseConsole();
                break;
            case 3:
                displayPriorityTriageQueue();
                pauseConsole();
                break;
            case 4:
                searchPatient();
                pauseConsole();
                break;
            case 5:
                updatePatientRecord();
                pauseConsole();
                break;
            case 6:
                displaySpecialties();
                pauseConsole();
                break;
            case 7:
                displayWards();
                pauseConsole();
                break;
            case 8:
                displayBedOccupancyMetrics();
                pauseConsole();
                break;
            case 9:
                allocateBed();
                pauseConsole();
                break;
            case 10:
                releaseBed();
                pauseConsole();
                break;
            case 11:
                generatePerformanceReport();
                pauseConsole();
                break;
            case 12:
                processBillCalculation();
                pauseConsole();
                break;
            case 13:
                printf("\nSaving data records to disk before exiting...\n");
                savePatientRecords();
                saveBedStatus();
                printf("Data saved successfully. Thank you for using Smart Hospital System.\n");
                return 0;
            default:
                break;
        }
    }

    return 0;
}
