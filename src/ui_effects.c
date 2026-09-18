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

void enableVTMode() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void displayStartupScreen() {
    enableVTMode();
    printf("\n%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|                                                          |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|             SMART HOSPITAL SYSTEM                        |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|                                                          |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|             Initializing System...                       |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|             [%s", COLOR_CYAN, COLOR_GREEN);
    fflush(stdout);

    int totalBlocks = 25;
    for (int i = 1; i <= totalBlocks; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(40);
    }
    printf("%s] 100%%       %s|\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|                                                          |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|                %sSYSTEM READY [OK]%s%s                         |%s\n", COLOR_CYAN, COLOR_GREEN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s|                                                          |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s+==========================================================+%s\n\n", COLOR_CYAN, COLOR_RESET);
    
    milliSleep(400);
}

void drawBoxHeader(const char *title) {
    printf("\n%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s| %-56s |%s\n", COLOR_CYAN, title, COLOR_RESET);
    printf("%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
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

    printf("\n%s%-40s%s [", COLOR_CYAN, label, COLOR_RESET);
    fflush(stdout);
    for (int i = 1; i <= totalSteps; i++) {
        printf("%s=", COLOR_GREEN);
        fflush(stdout);
        milliSleep(stepDelayMs);
    }
    printf("%s] 100%%%s\n", COLOR_GREEN, COLOR_RESET);
}

void showLoadingSpinner(const char *label, int durationMs) {
    static const char spinnerChars[] = "|/-\\";
    int frameDelay = 50;
    int totalFrames = durationMs / frameDelay;
    if (totalFrames <= 0) totalFrames = 8;

    printf("\n");
    for (int i = 0; i < totalFrames; i++) {
        printf("\r%s%-50s%s [%s%c%s]", COLOR_CYAN, label, COLOR_RESET, COLOR_YELLOW, spinnerChars[i % 4], COLOR_RESET);
        fflush(stdout);
        milliSleep(frameDelay);
    }
    printf("\r%s%-50s%s [%sOK%s]\n", COLOR_CYAN, label, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
}

void showBedScanningAnimation(int wardID) {
    printf("\n%s[Bed Matrix] Scanning Ward #%d Bed Availability:%s [ ", COLOR_CYAN, wardID, COLOR_RESET);
    fflush(stdout);
    for (int b = 1; b <= 5; b++) {
        printf("%sBed-%02d..%s ", COLOR_YELLOW, b, COLOR_RESET);
        fflush(stdout);
        milliSleep(60);
    }
    printf("] -> %sAVAILABLE BED FOUND [OK]%s\n", COLOR_GREEN, COLOR_RESET);
}

void showBedAllocationVisual(int bedID, const char *patientID, const char *patientName) {
    printf("\n%s+----------------------------------------------------------+%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s|              BED ALLOCATION CONFIRMATION                 |%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s+----------------------------------------------------------+%s\n", COLOR_GREEN, COLOR_RESET);
    printf("|  BED ID ASSIGNED:   %-36d |\n", bedID);
    printf("|  PATIENT ID:        %-36s |\n", patientID);
    printf("|  PATIENT NAME:      %-36s |\n", patientName);
    printf("|  STATUS:            %sRESERVED & OCCUPIED [CONFIRMED]%s      |\n", COLOR_GREEN, COLOR_RESET);
    printf("%s+----------------------------------------------------------+%s\n", COLOR_GREEN, COLOR_RESET);
}

void showTriageAlert(int emergencyStatus, const char *patientName) {
    if (emergencyStatus == 3) {
        printf("\n%s+==========================================================+%s\n", COLOR_RED, COLOR_RESET);
        printf("%s| [!!!] CRITICAL EMERGENCY ALERT (LEVEL 3) [!!!]           |%s\n", COLOR_RED, COLOR_RESET);
        printf("%s| Patient '%-28s' requires IMMEDIATE CARE |%s \n", COLOR_RED, patientName, COLOR_RESET);
        printf("%s+==========================================================+%s\n", COLOR_RED, COLOR_RESET);
    } else if (emergencyStatus == 2) {
        printf("\n%s+----------------------------------------------------------+%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| [!] URGENT TRIAGE NOTICE (LEVEL 2)                       |%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s| Patient '%-28s' assigned HIGH PRIORITY  |%s\n", COLOR_YELLOW, patientName, COLOR_RESET);
        printf("%s+----------------------------------------------------------+%s\n", COLOR_YELLOW, COLOR_RESET);
    }
    milliSleep(300);
}

void showTriageQueueSortingAnimation() {
    showLoadingSpinner("Sorting Triage Priority Queue (Critical -> Urgent -> Normal)", 350);
}

void showHospitalStatusDisplay(int totalPatients, int occupiedBeds, int totalBeds) {
    float occupancyPct = (totalBeds > 0) ? ((float)occupiedBeds / (float)totalBeds) * 100.0f : 0.0f;
    printf("\n%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s|              HOSPITAL REAL-TIME STATUS                   |%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
    printf("| Total Active Patients: %-33d |\n", totalPatients);
    printf("| Bed Occupancy Count:   %d / %-28d |\n", occupiedBeds, totalBeds);
    printf("| Bed Utilization Rate:  %s%-33.1f%%%s |\n", (occupancyPct > 80.0f ? COLOR_RED : occupancyPct > 50.0f ? COLOR_YELLOW : COLOR_GREEN), occupancyPct, COLOR_RESET);
    printf("%s+==========================================================+%s\n", COLOR_CYAN, COLOR_RESET);
}

void drawBarChart(const char *label, int count, int totalCount, int maxBarWidth) {
    if (maxBarWidth <= 0) maxBarWidth = 20;
    float pct = (totalCount > 0) ? ((float)count / (float)totalCount) * 100.0f : 0.0f;
    int filledBars = (totalCount > 0) ? (int)((float)count / (float)totalCount * maxBarWidth + 0.5f) : 0;
    if (filledBars > maxBarWidth) filledBars = maxBarWidth;

    const char *barColor = COLOR_GREEN;
    if (strstr(label, "Critical") != NULL || strstr(label, "ICU") != NULL) {
        barColor = COLOR_RED;
    } else if (strstr(label, "Urgent") != NULL || strstr(label, "Emergency") != NULL) {
        barColor = COLOR_YELLOW;
    }

    printf(" %-22s [", label);
    for (int i = 0; i < maxBarWidth; i++) {
        if (i < filledBars) {
            printf("%s*%s", barColor, COLOR_RESET);
        } else {
            printf(" ");
        }
    }
    printf("] %s%5.1f%%%s (%d)\n", barColor, pct, COLOR_RESET, count);
}

void displayColorLegend() {
    printf("\n%s[LEGEND]%s  %s[AVAILABLE / SUCCESS]%s  %s[URGENT / WARNING]%s  %s[OCCUPIED / CRITICAL]%s  %s[HEADINGS]%s\n",
           COLOR_BOLD, COLOR_RESET,
           COLOR_GREEN, COLOR_RESET,
           COLOR_YELLOW, COLOR_RESET,
           COLOR_RED, COLOR_RESET,
           COLOR_CYAN, COLOR_RESET);
}
