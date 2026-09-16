#include <stdio.h>
#include <string.h>
#include "patient.h"

Patient patients[MAX_PATIENTS];
int patientCount = 0;

void registerPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("\n[Error] Cannot register patient. Hospital system capacity reached (%d patients max).\n", MAX_PATIENTS);
        return;
    }

    Patient p;
    sprintf(p.id, "PAT-%04d", patientCount + 1001);

    printf("\n--- PATIENT REGISTRATION FORM (%s) ---\n", p.id);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Patient Name: ");
    if (fgets(p.name, sizeof(p.name), stdin) != NULL) {
        p.name[strcspn(p.name, "\n")] = '\0';
    }

    printf("Enter Age (1 to 120 years): ");
    if (scanf("%d", &p.age) != 1 || p.age < 1 || p.age > 120) {
        printf("[Validation Error] Invalid age entered. Registration cancelled for security.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    printf("Enter Gender (Male/Female/Other): ");
    scanf("%9s", p.gender);

    printf("Enter Contact Number: ");
    scanf("%19s", p.contact);

    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Medical Condition / Visit Reason: ");
    if (fgets(p.condition, sizeof(p.condition), stdin) != NULL) {
        p.condition[strcspn(p.condition, "\n")] = '\0';
    }

    printf("Enter Emergency Status (1 = Normal OPD, 2 = Urgent, 3 = Critical Emergency): ");
    if (scanf("%d", &p.emergencyStatus) != 1 || p.emergencyStatus < 1 || p.emergencyStatus > 3) {
        printf("[Note] Unrecognized status entered. Defaulting to Level 1 (Normal OPD).\n");
        p.emergencyStatus = 1;
    }

    p.wardID = 0;
    p.bedID = 0;

    patients[patientCount++] = p;
    
    printf("\n[Success] Patient '%s' registered successfully with assigned ID: %s!\n", p.name, p.id);
}

void displayPatients() {
    if (patientCount == 0) {
        printf("\n[Information] No patients are currently registered in the system.\n");
        return;
    }

    printf("\n=============================================================================================\n");
    printf("                                REGISTERED PATIENTS DIRECTORY                                \n");
    printf("=============================================================================================\n");
    printf("%-10s %-20s %-5s %-8s %-15s %-10s %-8s %-6s\n",
           "Patient ID", "Name", "Age", "Gender", "Contact", "Status", "Ward ID", "Bed ID");
    printf("---------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < patientCount; i++) {
        const char *statusStr = (patients[i].emergencyStatus == 3) ? "Critical" :
                                (patients[i].emergencyStatus == 2) ? "Urgent" : "Normal";
        
        printf("%-10s %-20s %-5d %-8s %-15s %-10s %-8d %-6d\n",
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].gender,
               patients[i].contact,
               statusStr,
               patients[i].wardID,
               patients[i].bedID);
    }
    printf("---------------------------------------------------------------------------------------------\n");
}

Patient* findPatientByID(const char* patientID) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(patients[i].id, patientID) == 0) {
            return &patients[i];
        }
    }
    return NULL;
}

int getPatientCount() {
    return patientCount;
}
