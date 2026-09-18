#ifndef UI_EFFECTS_H
#define UI_EFFECTS_H

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

#endif
