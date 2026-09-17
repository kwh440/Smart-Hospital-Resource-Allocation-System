#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 100

typedef struct {
    char id[15];
    char name[50];
    int age;
    char gender[10];
    char contact[20];
    char condition[50];
    int emergencyStatus;
    int wardID;
    int bedID;
} Patient;

extern Patient patients[MAX_PATIENTS];
extern int patientCount;

void clearInputBuffer();
int readIntBounded(const char *prompt, int minVal, int maxVal);
void readStringNonEmpty(const char *prompt, char *buffer, int maxLen);
int isDuplicatePatient(const char *name, const char *contact);

void registerPatient();
void displayPatients();
Patient* findPatientByID(const char* patientID);
int getPatientCount();

void sortPatientsByPriority(Patient tempArray[], int count);
void displayPriorityTriageQueue();
void searchPatient();
void updatePatientRecord();

#endif
