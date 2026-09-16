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
int findAvailableBed(int wardID);
void allocateBed();

#endif
