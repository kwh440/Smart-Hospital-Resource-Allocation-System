#ifndef UI_EFFECTS_H
#define UI_EFFECTS_H

/* ANSI Semantic Color Macros */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"

void enableVTMode();
void displayStartupScreen();
void drawBoxHeader(const char *title);
void pauseConsole();
void showProgressBar(const char *label, int totalSteps, int stepDelayMs);
void showLoadingSpinner(const char *label, int durationMs);

/* V7.3 Hospital Visual Effects */
void showBedScanningAnimation(int wardID);
void showBedAllocationVisual(int bedID, const char *patientID, const char *patientName);
void showTriageAlert(int emergencyStatus, const char *patientName);
void showTriageQueueSortingAnimation();
void showHospitalStatusDisplay(int totalPatients, int occupiedBeds, int totalBeds);

/* V7.4 Analytics & Bar Charts */
void drawBarChart(const char *label, int count, int totalCount, int maxBarWidth);

/* Visual Enhancements */
void displayColorLegend();

#endif
