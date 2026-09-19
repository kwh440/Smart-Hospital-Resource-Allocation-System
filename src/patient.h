/*
 * Smart Hospital & Resource Allocation System
 *
 * File: patient.h
 * Purpose: Header file for patient data structure definition, patient array declaration,
 *          and prototypes for patient intake, triage, searching, and updating.
 */

#ifndef PATIENT_H
#define PATIENT_H

/* ============================================================
   CONSTANTS AND CONFIGURATION
   ============================================================ */

#define MAX_PATIENTS 100

/* ============================================================
   DATA STRUCTURES
   ============================================================ */

/*
 * Structure: Patient
 * Purpose  : Encapsulates comprehensive patient record data including personal details,
 *            emergency triage level, selected specialty, ward admission, and bed assignment.
 *
 * Emergency Triage Levels:
 *   1 = Normal OPD (Standard priority)
 *   2 = Urgent (High priority)
 *   3 = Critical Emergency (Immediate care priority)
 */
typedef struct {
    char id[20];            /* Unique Patient ID (e.g. PAT-1001) */
    char name[50];          /* Patient Full Name */
    int age;                /* Patient Age in Years */
    char gender[10];        /* Gender (Male / Female) */
    char contact[20];       /* Contact Telephone Number */
    char condition[50];     /* Medical Condition / Visit Reason */
    int emergencyStatus;    /* Urgency Level: 1 = Normal, 2 = Urgent, 3 = Critical */
    int specialtyID;        /* Selected Doctor Specialty ID (1 to 7) */
    int isAdmitted;         /* Ward Admission Flag: 1 = Yes, 0 = No (OPD) */
    int wardID;             /* Assigned Ward ID (1 to 4, or 0 if OPD) */
    int bedID;              /* Assigned Bed ID (e.g. 101, 201, or 0 if OPD) */
    int daysAdmitted;       /* Duration of Stay in Ward (Days) */
} Patient;

/* ============================================================
   GLOBAL ARRAY & COUNTER DECLARATIONS
   ============================================================ */

extern Patient patients[MAX_PATIENTS];
extern int patientCount;

/* ============================================================
   INPUT VALIDATION HELPERS
   ============================================================ */

/* Cleans stdin input buffer to prevent buffer overflow and input loops. */
void clearInputBuffer();

/* Reads an integer from stdin and enforces minVal <= value <= maxVal bounds. */
int readIntBounded(const char *prompt, int minVal, int maxVal);

/* Reads a string from stdin ensuring non-empty input. */
void readStringNonEmpty(const char *prompt, char *buffer, int maxLen);

/* Prompts user to select Gender (1 = Male, 2 = Female). */
void readGender(char *buffer, int maxLen);

/* Checks if a patient with matching name and contact already exists. */
int isDuplicatePatient(const char *name, const char *contact);

/* ============================================================
   PATIENT MANAGEMENT & TRIAGE FUNCTIONS
   ============================================================ */

/* Registers a new patient with intake details, triage level, and optional bed allocation. */
void registerPatient();

/* Displays all registered patients in arrival / registration order. */
void displayPatients();

/* Finds a patient by unique ID string and returns a pointer to the record. */
Patient* findPatientByID(const char* patientID);

/* Returns the total number of registered patients. */
int getPatientCount();

/* Sorts a temporary array of patients by emergency priority (Level 3 -> Level 2 -> Level 1). */
void sortPatientsByPriority(Patient tempArray[], int count);

/* Displays priority triage queue ordered by urgency level. */
void displayPriorityTriageQueue();

/* Searches patient records by ID or case-insensitive partial name match. */
void searchPatient();

/* Updates existing patient record information interactively. */
void updatePatientRecord();

#endif
