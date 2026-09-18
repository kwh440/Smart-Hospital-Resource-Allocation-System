#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "patient.h"
#include "ui_effects.h"

Patient patients[MAX_PATIENTS];
int patientCount = 0;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readIntBounded(const char *prompt, int minVal, int maxVal) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1 && val >= minVal && val <= maxVal) {
            clearInputBuffer();
            return val;
        }
        printf("[Validation Error] Please enter a valid number between %d and %d.\n", minVal, maxVal);
        clearInputBuffer();
    }
}

void readStringNonEmpty(const char *prompt, char *buffer, int maxLen) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, maxLen, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (strlen(buffer) > 0) {
                return;
            }
        }
        printf("[Validation Error] Input cannot be empty. Please try again.\n");
    }
}

void readGender(char *buffer, int maxLen) {
    int choice = readIntBounded("Select Gender (1 = Male, 2 = Female): ", 1, 2);
    if (choice == 1) {
        strncpy(buffer, "Male", maxLen - 1);
    } else {
        strncpy(buffer, "Female", maxLen - 1);
    }
    buffer[maxLen - 1] = '\0';
}

int isDuplicatePatient(const char *name, const char *contact) {
    for (int i = 0; i < patientCount; i++) {
        if (strcasecmp(patients[i].name, name) == 0 && strcmp(patients[i].contact, contact) == 0) {
            return 1;
        }
    }
    return 0;
}

void registerPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("\n[Error] Cannot register patient. Hospital system capacity reached (%d patients max).\n", MAX_PATIENTS);
        return;
    }

    Patient p;
    sprintf(p.id, "PAT-%04d", patientCount + 1001);

    printf("\n--- PATIENT REGISTRATION FORM (%s) ---\n", p.id);

    readStringNonEmpty("Enter Patient Name: ", p.name, sizeof(p.name));

    readStringNonEmpty("Enter Contact Number: ", p.contact, sizeof(p.contact));

    if (isDuplicatePatient(p.name, p.contact)) {
        printf("\n[Duplicate Notice] A patient named '%s' with contact '%s' is already registered in the system.\n", p.name, p.contact);
        printf("Registration cancelled to prevent duplicate record entry.\n");
        return;
    }

    p.age = readIntBounded("Enter Age (1 to 120 years): ", 1, 120);

    readGender(p.gender, sizeof(p.gender));

    readStringNonEmpty("Enter Medical Condition / Visit Reason: ", p.condition, sizeof(p.condition));

    p.emergencyStatus = readIntBounded("Enter Emergency Status (1 = Normal OPD, 2 = Urgent, 3 = Critical Emergency): ", 1, 3);

    p.wardID = 0;
    p.bedID = 0;

    patients[patientCount++] = p;

    showLoadingSpinner("Processing Patient Registration...", 400);
    showTriageAlert(p.emergencyStatus, p.name);
    printf("\n%s[Success] Patient '%s' registered successfully with assigned ID: %s!%s\n", COLOR_GREEN, p.name, p.id, COLOR_RESET);
}

void displayPatients() {
    if (patientCount == 0) {
        printf("\n%s[Information] No patients are currently registered in the system.%s\n", COLOR_CYAN, COLOR_RESET);
        return;
    }

    printf("\n%s╔══════════════════════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                                    REGISTERED PATIENTS DIRECTORY                                     ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠══════════════╦══════════════════════╦═══════╦══════════╦═════════════════╦══════════════╦════════╦════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s Patient ID   %s║%s Name                 %s║%s Age   %s║%s Gender %s║%s Contact       %s║%s Status       %s║%s Ward   %s║%s Bed    %s║%s\n",
           COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠══════════════╬══════════════════════╬═══════╬══════════╬═════════════════╬══════════════╬════════╬════════╣%s\n", COLOR_CYAN, COLOR_RESET);

    for (int i = 0; i < patientCount; i++) {
        const char *statusStr = (patients[i].emergencyStatus == 3) ? "Critical" :
                                (patients[i].emergencyStatus == 2) ? "Urgent" : "Normal";
        const char *colorStr = (patients[i].emergencyStatus == 3) ? COLOR_RED :
                               (patients[i].emergencyStatus == 2) ? COLOR_YELLOW : COLOR_GREEN;

        printf("%s║%s %-12s %s║%s %-20s %s║%s %-5d %s║%s %-8s %s║%s %-15s %s║%s %s%-12s%s %s║%s %-6d %s║%s %-6d %s║%s\n",
               COLOR_CYAN, COLOR_RESET,
               patients[i].id,
               COLOR_CYAN, COLOR_RESET,
               patients[i].name,
               COLOR_CYAN, COLOR_RESET,
               patients[i].age,
               COLOR_CYAN, COLOR_RESET,
               patients[i].gender,
               COLOR_CYAN, COLOR_RESET,
               patients[i].contact,
               COLOR_CYAN, COLOR_RESET,
               colorStr, statusStr, COLOR_RESET,
               COLOR_CYAN, COLOR_RESET,
               patients[i].wardID,
               COLOR_CYAN, COLOR_RESET,
               patients[i].bedID,
               COLOR_CYAN, COLOR_RESET);
    }
    printf("%s╚══════════════╩══════════════════════╩═══════╩══════════╩═════════════════╩══════════════╩════════╩════════╝%s\n", COLOR_CYAN, COLOR_RESET);
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
    showTriageQueueSortingAnimation();
    displayColorLegend();

    printf("\n%s╔══════════════════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                                EMERGENCY TRIAGE PRIORITY QUEUE                                   ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠══════╦══════════════╦══════════════════════╦════════════════╦═══════╦══════════════════════╦════════╦════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s #    %s║%s Patient ID   %s║%s Name                 %s║%s Priority       %s║%s Age   %s║%s Medical Condition    %s║%s Ward   %s║%s Bed    %s║%s\n",
           COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠══════╬══════════════╬══════════════════════╬════════════════╬═══════╬══════════════════════╬════════╬════════╣%s\n", COLOR_CYAN, COLOR_RESET);

    for (int i = 0; i < patientCount; i++) {
        const char *statusStr = (tempQueue[i].emergencyStatus == 3) ? "CRITICAL" :
                                (tempQueue[i].emergencyStatus == 2) ? "URGENT" : "NORMAL";
        const char *colorStr = (tempQueue[i].emergencyStatus == 3) ? COLOR_RED :
                               (tempQueue[i].emergencyStatus == 2) ? COLOR_YELLOW : COLOR_GREEN;

        printf("%s║%s %-4d %s║%s %-12s %s║%s %-20s %s║%s %s%-14s%s %s║%s %-5d %s║%s %-20s %s║%s %-6d %s║%s %-6d %s║%s\n",
               COLOR_CYAN, COLOR_RESET,
               i + 1,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].id,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].name,
               COLOR_CYAN, COLOR_RESET,
               colorStr, statusStr, COLOR_RESET,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].age,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].condition,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].wardID,
               COLOR_CYAN, COLOR_RESET,
               tempQueue[i].bedID,
               COLOR_CYAN, COLOR_RESET);
    }
    printf("%s╚══════╩══════════════╩══════════════════════╩════════════════╩═══════╩══════════════════════╩════════╩════════╝%s\n", COLOR_CYAN, COLOR_RESET);
}

void searchPatient() {
    if (patientCount == 0) {
        printf("\n[Information] No patients registered to search.\n");
        return;
    }

    printf("\n--- SEARCH PATIENT RECORDS ---\n");
    printf("1. Search by Patient ID (e.g. PAT-1001)\n");
    printf("2. Search by Patient Name (Partial/Full Match)\n");
    int mode = readIntBounded("Select Option (1-2): ", 1, 2);

    if (mode == 1) {
        char searchID[15];
        readStringNonEmpty("Enter Patient ID to search: ", searchID, sizeof(searchID));

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
        readStringNonEmpty("Enter Patient Name (or partial name): ", searchName, sizeof(searchName));

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
    readStringNonEmpty("\nEnter Patient ID to update (e.g. PAT-1001): ", targetID, sizeof(targetID));

    Patient *p = findPatientByID(targetID);
    if (p == NULL) {
        printf("[Error] Patient '%s' not found.\n", targetID);
        return;
    }

    printf("\n--- CURRENT PATIENT RECORD FOR %s ---\n", p->id);
    printf("1. Name             : %s\n", p->name);
    printf("2. Age              : %d\n", p->age);
    printf("3. Gender           : %s\n", p->gender);
    printf("4. Contact Number   : %s\n", p->contact);
    printf("5. Condition        : %s\n", p->condition);
    printf("6. Emergency Status : %d (%s)\n", p->emergencyStatus, (p->emergencyStatus == 3) ? "Critical" : (p->emergencyStatus == 2) ? "Urgent" : "Normal");
    printf("---------------------------------------\n");

    char inputBuf[100];

    printf("Enter New Name (or press Enter to keep '%s'): ", p->name);
    if (fgets(inputBuf, sizeof(inputBuf), stdin) != NULL) {
        inputBuf[strcspn(inputBuf, "\n")] = '\0';
        if (strlen(inputBuf) > 0) {
            strcpy(p->name, inputBuf);
        }
    }

    p->age = readIntBounded("Enter New Age (1-120): ", 1, 120);

    printf("Select New Gender (1 = Male, 2 = Female, or 0 to keep '%s'): ", p->gender);
    int gChoice;
    if (scanf("%d", &gChoice) == 1 && (gChoice == 1 || gChoice == 2)) {
        strcpy(p->gender, (gChoice == 1) ? "Male" : "Female");
    }
    clearInputBuffer();

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

    p->emergencyStatus = readIntBounded("Enter New Emergency Status (1 = Normal, 2 = Urgent, 3 = Critical): ", 1, 3);

    printf("\n[Success] Patient record for '%s' updated successfully.\n", p->id);
}
