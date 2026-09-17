#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

void sortPatientsByPriority(Patient tempArray[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (tempArray[j].emergencyStatus < tempArray[j + 1].emergencyStatus) {
                Patient temp = tempArray[j];
                tempArray[j] = tempArray[j + 1];
                tempArray[j + 1] = temp;
            }
        }
    }
}

void displayPriorityTriageQueue() {
    if (patientCount == 0) {
        printf("\n[Information] No patients currently in triage queue.\n");
        return;
    }

    Patient tempQueue[MAX_PATIENTS];
    for (int i = 0; i < patientCount; i++) {
        tempQueue[i] = patients[i];
    }

    sortPatientsByPriority(tempQueue, patientCount);

    printf("\n=============================================================================================\n");
    printf("                       EMERGENCY TRIAGE PRIORITY QUEUE (V2)                                 \n");
    printf("=============================================================================================\n");
    printf("%-6s %-10s %-20s %-12s %-5s %-20s %-8s %-6s\n",
           "Rank", "Patient ID", "Name", "Urgency", "Age", "Condition", "Ward ID", "Bed ID");
    printf("---------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < patientCount; i++) {
        const char *statusStr = (tempQueue[i].emergencyStatus == 3) ? "3 (Critical)" :
                                (tempQueue[i].emergencyStatus == 2) ? "2 (Urgent)" : "1 (Normal)";
        
        printf("#%-5d %-10s %-20s %-12s %-5d %-20s %-8d %-6d\n",
               i + 1,
               tempQueue[i].id,
               tempQueue[i].name,
               statusStr,
               tempQueue[i].age,
               tempQueue[i].condition,
               tempQueue[i].wardID,
               tempQueue[i].bedID);
    }
    printf("---------------------------------------------------------------------------------------------\n");
}

void searchPatient() {
    if (patientCount == 0) {
        printf("\n[Information] No patients registered to search.\n");
        return;
    }

    int mode = 0;
    printf("\n--- SEARCH PATIENT RECORDS ---\n");
    printf("1. Search by Patient ID (e.g. PAT-1001)\n");
    printf("2. Search by Patient Name (Partial/Full Match)\n");
    printf("Select Option (1-2): ");
    
    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        printf("[Error] Invalid search option selected.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (mode == 1) {
        char searchID[15];
        printf("Enter Patient ID to search: ");
        scanf("%14s", searchID);

        Patient *p = findPatientByID(searchID);
        if (p == NULL) {
            printf("[Result] No patient found with ID '%s'.\n", searchID);
        } else {
            printf("\n--- PATIENT RECORD FOUND ---\n");
            printf("ID               : %s\n", p->id);
            printf("Name             : %s\n", p->name);
            printf("Age              : %d Years\n", p->age);
            printf("Gender           : %s\n", p->gender);
            printf("Contact          : %s\n", p->contact);
            printf("Condition        : %s\n", p->condition);
            printf("Emergency Status : %s\n", (p->emergencyStatus == 3) ? "Critical" : (p->emergencyStatus == 2) ? "Urgent" : "Normal");
            printf("Ward ID / Bed ID : %d / %d\n", p->wardID, p->bedID);
            printf("----------------------------\n");
        }
    } else {
        char searchName[50];
        printf("Enter Patient Name (or partial name): ");
        if (fgets(searchName, sizeof(searchName), stdin) != NULL) {
            searchName[strcspn(searchName, "\n")] = '\0';
        }

        if (strlen(searchName) == 0) {
            printf("[Error] Search name cannot be empty.\n");
            return;
        }

        int foundCount = 0;
        printf("\n=== SEARCH RESULTS FOR '%s' ===\n", searchName);
        printf("%-10s %-20s %-5s %-15s %-10s\n", "Patient ID", "Name", "Age", "Contact", "Status");
        printf("----------------------------------------------------------------\n");

        for (int i = 0; i < patientCount; i++) {
            char pNameLower[50], searchLower[50];
            strcpy(pNameLower, patients[i].name);
            strcpy(searchLower, searchName);

            for (int k = 0; pNameLower[k]; k++) pNameLower[k] = (char)tolower((unsigned char)pNameLower[k]);
            for (int k = 0; searchLower[k]; k++) searchLower[k] = (char)tolower((unsigned char)searchLower[k]);

            if (strstr(pNameLower, searchLower) != NULL) {
                const char *statusStr = (patients[i].emergencyStatus == 3) ? "Critical" :
                                        (patients[i].emergencyStatus == 2) ? "Urgent" : "Normal";
                printf("%-10s %-20s %-5d %-15s %-10s\n",
                       patients[i].id, patients[i].name, patients[i].age, patients[i].contact, statusStr);
                foundCount++;
            }
        }
        printf("----------------------------------------------------------------\n");
        printf("Total records found: %d\n", foundCount);
    }
}

void updatePatientRecord() {
    if (patientCount == 0) {
        printf("\n[Information] No patients registered to update.\n");
        return;
    }

    char targetID[15];
    printf("\nEnter Patient ID to update (e.g. PAT-1001): ");
    scanf("%14s", targetID);

    Patient *p = findPatientByID(targetID);
    if (p == NULL) {
        printf("[Error] Patient '%s' not found.\n", targetID);
        return;
    }

    printf("\n--- CURRENT PATIENT RECORD FOR %s ---\n", p->id);
    printf("1. Name             : %s\n", p->name);
    printf("2. Age              : %d\n", p->age);
    printf("3. Contact Number   : %s\n", p->contact);
    printf("4. Condition        : %s\n", p->condition);
    printf("5. Emergency Status : %d (%s)\n", p->emergencyStatus, (p->emergencyStatus == 3) ? "Critical" : (p->emergencyStatus == 2) ? "Urgent" : "Normal");
    printf("---------------------------------------\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char inputBuf[100];

    printf("Enter New Name (or press Enter to keep '%s'): ", p->name);
    if (fgets(inputBuf, sizeof(inputBuf), stdin) != NULL) {
        inputBuf[strcspn(inputBuf, "\n")] = '\0';
        if (strlen(inputBuf) > 0) {
            strcpy(p->name, inputBuf);
        }
    }

    printf("Enter New Age (1-120, or enter 0 to keep %d): ", p->age);
    int newAge;
    if (scanf("%d", &newAge) == 1 && newAge >= 1 && newAge <= 120) {
        p->age = newAge;
    }
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter New Contact Number (or press Enter to keep '%s'): ", p->contact);
    if (fgets(inputBuf, sizeof(inputBuf), stdin) != NULL) {
        inputBuf[strcspn(inputBuf, "\n")] = '\0';
        if (strlen(inputBuf) > 0) {
            strcpy(p->contact, inputBuf);
        }
    }

    printf("Enter New Condition (or press Enter to keep '%s'): ", p->condition);
    if (fgets(inputBuf, sizeof(inputBuf), stdin) != NULL) {
        inputBuf[strcspn(inputBuf, "\n")] = '\0';
        if (strlen(inputBuf) > 0) {
            strcpy(p->condition, inputBuf);
        }
    }

    printf("Enter New Emergency Status (1 = Normal, 2 = Urgent, 3 = Critical, or 0 to keep %d): ", p->emergencyStatus);
    int newStatus;
    if (scanf("%d", &newStatus) == 1 && newStatus >= 1 && newStatus <= 3) {
        p->emergencyStatus = newStatus;
    }
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n[Success] Patient record for '%s' updated successfully!\n", p->id);
}
