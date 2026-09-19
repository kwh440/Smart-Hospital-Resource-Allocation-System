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
    printf("\n");
    printf("%s╔════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║        SMART HOSPITAL RESOURCE SYSTEM (V8.0)       ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║  %s[ PATIENT MANAGEMENT & TRIAGE ]%s                 ║%s\n", COLOR_CYAN, COLOR_YELLOW, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  1. Register New Patient                           %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  2. Display Patients Directory                     %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  3. Display Priority Triage Queue                  %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  4. Search Patient Record                          %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  5. Update Patient Record                          %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║  %s[ WARD & BED MANAGEMENT ]%s                       ║%s\n", COLOR_CYAN, COLOR_YELLOW, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  6. Display Doctor Specialties (7 Specialties)    %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  7. Display Hospital Wards (4 Wards)              %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  8. Display Ward Bed Status Tables                 %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  9. Allocate Bed (With AI Recommendation)         %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 10. Discharge Patient & Release Bed                %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║  %s[ FINANCIALS & ANALYTICS ]%s                      ║%s\n", COLOR_CYAN, COLOR_YELLOW, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 11. Generate Performance Reports & Analytics       %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 12. Calculate Admission Bill & Export Receipt      %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s 13. Save Data & Exit System                        %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
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
