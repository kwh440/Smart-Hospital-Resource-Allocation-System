/*
 * Smart Hospital & Resource Allocation System
 *
 * File: ui_effects.h
 * Purpose: Header file for ANSI console color definitions, startup animations, progress bars,
 *          spinners, triage alert displays, and graphical ASCII block charts.
 */

#ifndef UI_EFFECTS_H
#define UI_EFFECTS_H

/* ============================================================
   CONSOLE UI / ANSI COLOR CONSTANTS
   ============================================================
   ANSI Escape Sequences for terminal styling:
   - COLOR_RESET : Restores standard terminal font color
   - COLOR_BOLD  : Bold text emphasis
   - COLOR_RED   : Error, Critical urgency, Occupied status
   - COLOR_GREEN : Success, Normal status, Available state
   - COLOR_YELLOW: Warning, Urgent status
   - COLOR_CYAN  : Table borders, section headers, titles
   ============================================================ */

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */

/* Enables Virtual Terminal Processing for Windows CMD / PowerShell ANSI color support. */
void enableVTMode();

/* Displays animated startup initialization screen. */
void displayStartupScreen();

/* Draws double-line boxed section header. */
void drawBoxHeader(const char *title);

/* Pauses console output until user presses Enter. */
void pauseConsole();

/* Displays smooth progress bar during data operations. */
void showProgressBar(const char *label, int totalSteps, int stepDelayMs);

/* Displays smooth loading spinner animation (3-second duration). */
void showLoadingSpinner(const char *label, int durationMs);

/* Displays bed matrix availability scanning animation. */
void showBedScanningAnimation(int wardID);

/* Displays bed allocation visual confirmation banner. */
void showBedAllocationVisual(int bedID, const char *patientID, const char *patientName);

/* Displays emergency triage alert notice box. */
void showTriageAlert(int emergencyStatus, const char *patientName);

/* Displays priority queue sorting animation. */
void showTriageQueueSortingAnimation();

/* Displays real-time hospital occupancy status summary box. */
void showHospitalStatusDisplay(int totalPatients, int occupiedBeds, int totalBeds);

/* Renders solid color-coded ASCII block progress bar (█). */
void drawBarChart(const char *label, int count, int totalCount, int maxBarWidth);

/* Displays visual color key legend. */
void displayColorLegend();

#endif
