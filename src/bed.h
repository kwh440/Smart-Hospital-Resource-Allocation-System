#ifndef BED_H
#define BED_H

#define TOTAL_BEDS_IN_HOSPITAL 45

typedef struct {
    int bedID;
    int wardID;
    int status;
    char assignedPatientID[15];
} Bed;

extern Bed beds[TOTAL_BEDS_IN_HOSPITAL];

void initBeds();
void displayBeds();
void displayBedMatrix();
int findAvailableBed(int wardID);
void allocateBed();

int recommendWard(int emergencyStatus, int age);
void releaseBed();
void displayBedOccupancyMetrics();

/* Visual Enhancements */
void displayBoxedWardBedStatus();
void displayGraphicalBedCards();

#endif
