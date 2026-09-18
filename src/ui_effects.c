#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_effects.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static void milliSleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void displayStartupScreen() {
    printf("\n+==========================================================+\n");
    printf("|                                                          |\n");
    printf("|             SMART HOSPITAL SYSTEM                        |\n");
    printf("|                                                          |\n");
    printf("|             Initializing System...                       |\n");
    printf("|             [");
    fflush(stdout);

    int totalBlocks = 25;
    for (int i = 1; i <= totalBlocks; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(40);
    }
    printf("] 100%%       |\n");
    printf("|                                                          |\n");
    printf("|                SYSTEM READY [OK]                         |\n");
    printf("|                                                          |\n");
    printf("+==========================================================+\n\n");
    
    milliSleep(400);
}

void drawBoxHeader(const char *title) {
    printf("\n+==========================================================+\n");
    printf("| %-56s |\n", title);
    printf("+==========================================================+\n");
}

void pauseConsole() {
    printf("\n[Press ENTER to return to menu...]");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void showProgressBar(const char *label, int totalSteps, int stepDelayMs) {
    if (totalSteps <= 0) totalSteps = 20;
    if (stepDelayMs <= 0) stepDelayMs = 15;

    printf("\n%s [", label);
    fflush(stdout);
    for (int i = 1; i <= totalSteps; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(stepDelayMs);
    }
    printf("] 100%%\n");
}

void showLoadingSpinner(const char *label, int durationMs) {
    static const char spinnerChars[] = "|/-\\";
    int frameDelay = 50;
    int totalFrames = durationMs / frameDelay;
    if (totalFrames <= 0) totalFrames = 8;

    printf("\n");
    for (int i = 0; i < totalFrames; i++) {
        printf("\r%-50s [%c]", label, spinnerChars[i % 4]);
        fflush(stdout);
        milliSleep(frameDelay);
    }
    printf("\r%-50s [OK]\n", label);
    fflush(stdout);
}

void showBedScanningAnimation(int wardID) {
    printf("\n[Bed Matrix] Scanning Ward #%d Bed Availability: [ ", wardID);
    fflush(stdout);
    for (int b = 1; b <= 5; b++) {
        printf("Bed-%02d.. ", b);
        fflush(stdout);
        milliSleep(60);
    }
    printf("] -> AVAILABLE BED FOUND [OK]\n");
}

void showBedAllocationVisual(int bedID, const char *patientID, const char *patientName) {
    printf("\n+----------------------------------------------------------+\n");
    printf("|              BED ALLOCATION CONFIRMATION                 |\n");
    printf("+----------------------------------------------------------+\n");
    printf("|  BED ID ASSIGNED:   %-36d |\n", bedID);
    printf("|  PATIENT ID:        %-36s |\n", patientID);
    printf("|  PATIENT NAME:      %-36s |\n", patientName);
    printf("|  STATUS:            RESERVED & OCCUPIED [CONFIRMED]      |\n");
    printf("+----------------------------------------------------------+\n");
}

void showTriageAlert(int emergencyStatus, const char *patientName) {
    if (emergencyStatus == 3) {
        printf("\n+==========================================================+\n");
        printf("| [!!!] CRITICAL EMERGENCY ALERT (LEVEL 3) [!!!]           |\n");
        printf("| Patient '%-28s' requires IMMEDIATE CARE | \n", patientName);
        printf("+==========================================================+\n");
    } else if (emergencyStatus == 2) {
        printf("\n+----------------------------------------------------------+\n");
        printf("| [!] URGENT TRIAGE NOTICE (LEVEL 2)                       |\n");
        printf("| Patient '%-28s' assigned HIGH PRIORITY  |\n", patientName);
        printf("+----------------------------------------------------------+\n");
    }
    milliSleep(300);
}

void showTriageQueueSortingAnimation() {
    showLoadingSpinner("Sorting Triage Priority Queue (Critical -> Urgent -> Normal)", 350);
}

void showHospitalStatusDisplay(int totalPatients, int occupiedBeds, int totalBeds) {
    float occupancyPct = (totalBeds > 0) ? ((float)occupiedBeds / (float)totalBeds) * 100.0f : 0.0f;
    printf("\n+==========================================================+\n");
    printf("|              HOSPITAL REAL-TIME STATUS                   |\n");
    printf("+==========================================================+\n");
    printf("| Total Active Patients: %-33d |\n", totalPatients);
    printf("| Bed Occupancy Count:   %d / %-28d |\n", occupiedBeds, totalBeds);
    printf("| Bed Utilization Rate:  %-33.1f%% |\n", occupancyPct);
    printf("+==========================================================+\n");
}
