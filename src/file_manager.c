#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "patient.h"
#include "bed.h"
#include "ward.h"
#include "ui_effects.h"

#define PATIENT_FILE "data/patients.txt"
#define BED_FILE "data/beds.txt"

void initDirectoriesAndFiles() {
    FILE *fp = fopen(PATIENT_FILE, "a");
    if (fp != NULL) {
        fclose(fp);
    } else {
        #ifdef _WIN32
        system("mkdir data 2> NUL");
        #else
        system("mkdir -p data 2>/dev/null");
        #endif
    }
}

void savePatientRecords() {
    FILE *fp = fopen(PATIENT_FILE, "w");
    if (fp == NULL) {
        printf("[Warning] Unable to open '%s' for saving patient records.\n", PATIENT_FILE);
        return;
    }

    fprintf(fp, "%d\n", patientCount);

    for (int i = 0; i < patientCount; i++) {
        fprintf(fp, "%s;%s;%d;%s;%s;%s;%d;%d;%d\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].gender,
                patients[i].contact,
                patients[i].condition,
                patients[i].emergencyStatus,
                patients[i].wardID,
                patients[i].bedID);
    }

    fclose(fp);
    showProgressBar("[File Storage] Saving patient records...", 20, 15);
    printf("[File Storage] Saved %d patient record(s) to '%s'.\n", patientCount, PATIENT_FILE);
}

void loadPatientRecords() {
    FILE *fp = fopen(PATIENT_FILE, "r");
    if (fp == NULL) {
        return;
    }

    int count = 0;
    if (fscanf(fp, "%d\n", &count) != 1) {
        fclose(fp);
        return;
    }

    patientCount = 0;
    for (int i = 0; i < count && i < MAX_PATIENTS; i++) {
        Patient p;
        char line[256];
        if (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            
            char *token = strtok(line, ";");
            if (token) strcpy(p.id, token);
            
            token = strtok(NULL, ";");
            if (token) strcpy(p.name, token);

            token = strtok(NULL, ";");
            if (token) p.age = atoi(token);

            token = strtok(NULL, ";");
            if (token) strcpy(p.gender, token);

            token = strtok(NULL, ";");
            if (token) strcpy(p.contact, token);

            token = strtok(NULL, ";");
            if (token) strcpy(p.condition, token);

            token = strtok(NULL, ";");
            if (token) p.emergencyStatus = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.wardID = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.bedID = atoi(token);

            patients[patientCount++] = p;
        }
    }

    fclose(fp);
    showProgressBar("[File Storage] Loading patient records...", 20, 15);
    printf("[File Storage] Successfully loaded %d patient record(s) from '%s'.\n", patientCount, PATIENT_FILE);
}

void saveBedStatus() {
    FILE *fp = fopen(BED_FILE, "w");
    if (fp == NULL) {
        printf("[Warning] Unable to open '%s' for saving bed status.\n", BED_FILE);
        return;
    }

    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        fprintf(fp, "%d;%d;%d;%s\n",
                beds[i].bedID,
                beds[i].wardID,
                beds[i].status,
                beds[i].assignedPatientID);
    }

    fclose(fp);
    showProgressBar("[File Storage] Saving bed matrix status...", 20, 15);
    printf("[File Storage] Saved bed matrix status to '%s'.\n", BED_FILE);
}

void loadBedStatus() {
    FILE *fp = fopen(BED_FILE, "r");
    if (fp == NULL) {
        return;
    }

    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        char line[128];
        if (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            
            char *token = strtok(line, ";");
            if (token) beds[i].bedID = atoi(token);

            token = strtok(NULL, ";");
            if (token) beds[i].wardID = atoi(token);

            token = strtok(NULL, ";");
            if (token) beds[i].status = atoi(token);

            token = strtok(NULL, ";");
            if (token) strcpy(beds[i].assignedPatientID, token);

            if (beds[i].status == 1) {
                Ward *w = getWardByID(beds[i].wardID);
                if (w != NULL && w->availableBeds > 0) {
                    w->availableBeds--;
                }
            }
        }
    }

    fclose(fp);
    showProgressBar("[File Storage] Loading bed matrix status...", 20, 15);
    printf("[File Storage] Successfully loaded bed matrix from '%s'.\n", BED_FILE);
}
