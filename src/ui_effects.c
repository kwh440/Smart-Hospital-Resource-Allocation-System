/*
 * Smart Hospital & Resource Allocation System
 *
 * File: ui_effects.c
 * Purpose: Cross-platform timer delays, VT100 virtual terminal initialization,
 *          animated startup screens, progress bars, spinners, and graphical ASCII block charts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_effects.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* ============================================================
   CROSS-PLATFORM HELPER FUNCTIONS
   ============================================================ */

/*
 * Function: milliSleep
 * Purpose : Provides cross-platform millisecond thread pause utility.
 * Input   : milliseconds - Delay duration in milliseconds.
 * Returns : None
 */
static void milliSleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

/*
 * Function: enableVTMode
 * Purpose : Enables Virtual Terminal Processing and UTF-8 code page (65001) on Windows CMD / PowerShell
 *           to display ANSI colors and unicode box borders.
 * Input   : None
 * Returns : None
 */
void enableVTMode() {
#ifdef _WIN32
    SetConsoleOutputCP(65001); /* Set console code page to UTF-8 */
    SetConsoleCP(65001);
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

/* ============================================================
   STARTUP & INTERACTION VISUALS
   ============================================================ */

/*
 * Function: displayStartupScreen
 * Purpose : Displays animated startup initialization screen with progress bar.
 * Input   : None
 * Returns : None
 */
void displayStartupScreen() {
    enableVTMode();
    printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                                                    ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║             SMART HOSPITAL SYSTEM                  ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                                                    ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║             Initializing System...                 ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║             [%s", COLOR_CYAN, COLOR_GREEN);
    fflush(stdout);

    int totalBlocks = 25;
    for (int i = 1; i <= totalBlocks; i++) {
        printf("=");
        fflush(stdout);
        milliSleep(40);
    }
    printf("%s] 100%%       %s║%s\n", COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
    printf("%s║                                                    ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                %sSYSTEM READY [OK]%s%s                   ║%s\n", COLOR_CYAN, COLOR_GREEN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║                                                    ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);

    milliSleep(400);
}

/*
 * Function: drawBoxHeader
 * Purpose : Prints a double-line cyan boxed header banner.
 * Input   : title - Text label inside header box.
 * Returns : None
 */
void drawBoxHeader(const char *title) {
    printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║ %-50s ║%s\n", COLOR_CYAN, title, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
}

/*
 * Function: pauseConsole
 * Purpose : Pauses execution until the user presses Enter key.
 * Input   : None
 * Returns : None
 */
void pauseConsole() {
    printf("\n[Press ENTER to return to menu...]");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ============================================================
   PROGRESS BARS & ANIMATED SPINNERS
   ============================================================ */

/*
 * Function: showProgressBar
 * Purpose : Displays a colored progress bar for file storage or long operations.
 * Input   : label        - Operation description text.
 *           totalSteps   - Total progress step count.
 *           stepDelayMs  - Delay in milliseconds per step.
 * Returns : None
 */
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

/*
 * Function: showLoadingSpinner
 * Purpose : Displays a 3-second animated loading spinner (| / - \) for billing and sorting.
 * Input   : label      - Loading prompt text.
 *           durationMs - Total duration in milliseconds (enforced minimum 3000ms).
 * Returns : None
 */
void showLoadingSpinner(const char *label, int durationMs) {
    if (durationMs < 3000) durationMs = 3000; /* Minimum 3 seconds duration */
    static const char spinnerChars[] = "|/-\\";
    int frameDelay = 120;
    int totalFrames = durationMs / frameDelay;

    printf("\n");
    for (int i = 0; i < totalFrames; i++) {
        printf("\r%s%-50s%s [%s%c%s]", COLOR_CYAN, label, COLOR_RESET, COLOR_YELLOW, spinnerChars[i % 4], COLOR_RESET);
        fflush(stdout);
        milliSleep(frameDelay);
    }
    printf("\r%s%-50s%s [%sOK%s]\n", COLOR_CYAN, label, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
}

/* ============================================================
   HOSPITAL VISUAL EFFECTS & ALERTS
   ============================================================ */

/*
 * Function: showBedScanningAnimation
 * Purpose : Renders a scanning progress animation for finding free beds in a ward.
 * Input   : wardID - Target ward ID.
 * Returns : None
 */
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

/*
 * Function: showBedAllocationVisual
 * Purpose : Renders a double-line confirmation banner when a bed is allocated.
 * Input   : bedID       - Assigned bed ID.
 *           patientID   - Patient ID string.
 *           patientName - Patient full name.
 * Returns : None
 */
void showBedAllocationVisual(int bedID, const char *patientID, const char *patientName) {
    printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s║              BED ALLOCATION CONFIRMATION           ║%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s║%s  BED ID ASSIGNED:   %-28d %s║%s\n", COLOR_GREEN, COLOR_RESET, bedID, COLOR_GREEN, COLOR_RESET);
    printf("%s║%s  PATIENT ID:        %-28s %s║%s\n", COLOR_GREEN, COLOR_RESET, patientID, COLOR_GREEN, COLOR_RESET);
    printf("%s║%s  PATIENT NAME:      %-28s %s║%s\n", COLOR_GREEN, COLOR_RESET, patientName, COLOR_GREEN, COLOR_RESET);
    printf("%s║%s  STATUS:            %sRESERVED & OCCUPIED [CONFIRMED]%s %s║%s\n", COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_GREEN, COLOR_RESET);
}

/*
 * Function: showTriageAlert
 * Purpose : Displays emergency triage alert notice box color-coded by urgency level:
 *           - Level 3 Critical : RED
 *           - Level 2 Urgent   : YELLOW
 *           - Level 1 Normal   : GREEN
 * Input   : emergencyStatus - Urgency status level (1, 2, 3).
 *           patientName     - Registered patient name.
 * Returns : None
 */
void showTriageAlert(int emergencyStatus, const char *patientName) {
    if (emergencyStatus == 3) {
        printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_RED, COLOR_RESET);
        printf("%s║ [!!!] CRITICAL EMERGENCY ALERT (LEVEL 3)           ║%s\n", COLOR_RED, COLOR_RESET);
        printf("%s║ Patient: %-41s ║%s\n", COLOR_RED, patientName, COLOR_RESET);
        printf("%s║ Assigned: IMMEDIATE CARE PRIORITY                  ║%s\n", COLOR_RED, COLOR_RESET);
        printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_RED, COLOR_RESET);
    } else if (emergencyStatus == 2) {
        printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s║ [!] URGENT TRIAGE NOTICE (LEVEL 2)                 ║%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s║ Patient: %-41s ║%s\n", COLOR_YELLOW, patientName, COLOR_RESET);
        printf("%s║ Assigned: HIGH PRIORITY                            ║%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_YELLOW, COLOR_RESET);
    } else {
        printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_GREEN, COLOR_RESET);
        printf("%s║ [i] NORMAL TRIAGE NOTICE (LEVEL 1)                 ║%s\n", COLOR_GREEN, COLOR_RESET);
        printf("%s║ Patient: %-41s ║%s\n", COLOR_GREEN, patientName, COLOR_RESET);
        printf("%s║ Assigned: STANDARD OPD QUEUE                       ║%s\n", COLOR_GREEN, COLOR_RESET);
        printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_GREEN, COLOR_RESET);
    }
    milliSleep(300);
}

/*
 * Function: showTriageQueueSortingAnimation
 * Purpose : Triggers priority queue sorting animation.
 * Input   : None
 * Returns : None
 */
void showTriageQueueSortingAnimation() {
    showLoadingSpinner("Sorting Triage Priority Queue (Critical -> Urgent -> Normal)", 3000);
}

/*
 * Function: showHospitalStatusDisplay
 * Purpose : Renders real-time hospital statistics box.
 * Input   : totalPatients - Total registered patient count.
 *           occupiedBeds  - Total occupied bed count.
 *           totalBeds     - Total hospital bed capacity.
 * Returns : None
 */
void showHospitalStatusDisplay(int totalPatients, int occupiedBeds, int totalBeds) {
    float occupancyPct = (totalBeds > 0) ? ((float)occupiedBeds / (float)totalBeds) * 100.0f : 0.0f;
    const char *pctColor = (occupancyPct > 80.0f ? COLOR_RED : occupancyPct > 50.0f ? COLOR_YELLOW : COLOR_GREEN);

    char line1[60], line2[60], line3[60];
    snprintf(line1, sizeof(line1), "Total Active Patients: %d", totalPatients);
    snprintf(line2, sizeof(line2), "Bed Occupancy Count:   %d / %d", occupiedBeds, totalBeds);
    snprintf(line3, sizeof(line3), "Bed Utilization Rate:  %.1f %%", occupancyPct);

    printf("\n%s╔════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║              HOSPITAL REAL-TIME STATUS             ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  %-48s  %s║%s\n", COLOR_CYAN, COLOR_RESET, line1, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  %-48s  %s║%s\n", COLOR_CYAN, COLOR_RESET, line2, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  %s%-48s%s  %s║%s\n", COLOR_CYAN, COLOR_RESET, pctColor, line3, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
}

/* ============================================================
   GRAPHICAL ASCII BLOCK CHARTS
   ============================================================ */

/*
 * Function: drawBarChart
 * Purpose : Renders horizontal solid ASCII block bar charts (█) color-coded by metric:
 *           - Critical / ICU   : RED
 *           - Urgent / Surgical: YELLOW
 *           - Normal / General : GREEN
 * Input   : label       - Category metric title.
 *           count       - Current item count.
 *           totalCount  - Total category count.
 *           maxBarWidth - Maximum block bar character width.
 * Returns : None
 */
void drawBarChart(const char *label, int count, int totalCount, int maxBarWidth) {
    if (maxBarWidth <= 0) maxBarWidth = 25;
    float pct = (totalCount > 0) ? ((float)count / (float)totalCount) * 100.0f : 0.0f;
    int filledBars = (totalCount > 0) ? (int)((float)count / (float)totalCount * maxBarWidth + 0.5f) : 0;
    if (filledBars > maxBarWidth) filledBars = maxBarWidth;

    const char *barColor = COLOR_GREEN;
    if (strstr(label, "Critical") != NULL || strstr(label, "ICU") != NULL) {
        barColor = COLOR_RED;
    } else if (strstr(label, "Urgent") != NULL || strstr(label, "Surgical") != NULL || strstr(label, "Emergency") != NULL) {
        barColor = COLOR_YELLOW;
    }

    printf(" %-24s %s[%s", label, COLOR_CYAN, COLOR_RESET);
    for (int i = 0; i < maxBarWidth; i++) {
        if (i < filledBars) {
            printf("%s█%s", barColor, COLOR_RESET);
        } else {
            printf(" ");
        }
    }
    printf("%s] %s%5.1f%%%s (%d)\n", COLOR_CYAN, barColor, pct, COLOR_RESET, count);
}

/*
 * Function: displayColorLegend
 * Purpose : Displays visual key guide for ANSI terminal status colors.
 * Input   : None
 * Returns : None
 */
void displayColorLegend() {
    printf("\n%s[LEGEND]%s  %s[AVAILABLE / SUCCESS]%s  %s[URGENT / WARNING]%s  %s[OCCUPIED / CRITICAL]%s  %s[HEADINGS]%s\n",
           COLOR_BOLD, COLOR_RESET,
           COLOR_GREEN, COLOR_RESET,
           COLOR_YELLOW, COLOR_RESET,
           COLOR_RED, COLOR_RESET,
           COLOR_CYAN, COLOR_RESET);
}
