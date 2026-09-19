#ifndef WARD_H
#define WARD_H

#define MAX_WARDS 4
#define MAX_SPECIALTIES 7

typedef struct {
    int wardID;
    char name[30];
    float dailyRate;
    int totalBeds;
    int availableBeds;
} Ward;

typedef struct {
    int specialtyID;
    char name[40];
    float baseFee;
    int consultationTime; // Average consultation time in minutes
    int dailyCap;         // Max daily patient limit
} Specialty;

extern Ward wards[MAX_WARDS];
extern Specialty specialties[MAX_SPECIALTIES];

void initWardsAndSpecialties();
void displayWards();
void displaySpecialties();
Ward* getWardByID(int wardID);

#endif
