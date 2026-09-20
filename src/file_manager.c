/*
 * Smart Hospital & Resource Allocation System
 *
 * File: file_manager.c
 * Purpose: Implementation of text file handling for permanent persistence of patient records
 *          (data/patient_records.txt) and hospital bed matrix status (data/beds_status.txt).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "patient.h"
#include "bed.h"
#include "ward.h"
#include "ui_effects.h"

/* ============================================================
   FILE PERSISTENCE PATH MACROS
   ============================================================ */

#define PATIENT_FILE "data/patient_records.txt"
#define BED_FILE "data/beds_status.txt"

/* ============================================================
   DIRECTORY & FILE INITIALIZATION
   ============================================================ */

/*
 * Function: initDirectoriesAndFiles
 * Purpose : Ensures the data directory exists and checks read/write access for persistent files.
 * Input   : None
 * Returns : None
 */
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

/* ============================================================
   PATIENT RECORDS FILE OPERATIONS
   ============================================================ */

/*
 * Function: savePatientRecords
 * Purpose : Writes all active patient records from global array into data/patient_records.txt
 *           using semicolon (;) delimited formatting.
 * Input   : None
 * Returns : None
 */
void savePatientRecords() {
    FILE *fp = fopen(PATIENT_FILE, "w");
    if (fp == NULL) {
        printf("[Warning] Unable to open '%s' for saving patient records.\n", PATIENT_FILE);
        return;
    }

    /* Write total patient count header */
    fprintf(fp, "%d\n", patientCount);

    /* Write individual patient records (semicolon delimited tokens) */
    for (int i = 0; i < patientCount; i++) {
        fprintf(fp, "%s;%s;%d;%s;%s;%s;%d;%d;%d;%d;%d;%d\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].gender,
                patients[i].contact,
                patients[i].condition,
                patients[i].emergencyStatus,
                patients[i].specialtyID,
                patients[i].isAdmitted,
                patients[i].wardID,
                patients[i].bedID,
                patients[i].daysAdmitted);
    }

    fclose(fp);
    showProgressBar("[File Storage] Saving patient records...", 20, 15);
    printf("[File Storage] Saved %d patient record(s) to the database.\n", patientCount);
}

/*
 * Function: loadPatientRecords
 * Purpose : Reads patient record tokens from data/patient_records.txt during system startup
 *           and restores global patients array state.
 * Input   : None
 * Returns : None
 */
void loadPatientRecords() {
    FILE *fp = fopen(PATIENT_FILE, "r");
    if (fp == NULL) {
        return; /* File does not exist yet (first run) */
    }

    int count = 0;
    if (fscanf(fp, "%d\n", &count) != 1) {
        fclose(fp);
        return;
    }

    patientCount = 0;
    for (int i = 0; i < count && i < MAX_PATIENTS; i++) {
        Patient p;
        memset(&p, 0, sizeof(Patient));
        char line[256];
        if (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\r\n")] = '\0'; /* Strip newline characters */
            
            /* Tokenize line by semicolon (;) delimiter */
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
            if (token) p.specialtyID = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.isAdmitted = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.wardID = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.bedID = atoi(token);

            token = strtok(NULL, ";");
            if (token) p.daysAdmitted = atoi(token);

            patients[patientCount++] = p;
        }
    }

    fclose(fp);
    showProgressBar("[File Storage] Loading patient records...", 20, 15);
    printf("[File Storage] Successfully loaded %d patient record(s) from the database.\n", patientCount);
}

/* ============================================================
   BED STATUS FILE OPERATIONS
   ============================================================ */

/*
 * Function: saveBedStatus
 * Purpose : Writes complete 45-bed hospital occupancy state to data/beds_status.txt.
 * Input   : None
 * Returns : None
 */
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
    printf("[File Storage] Saved bed matrix status to the database.\n");
}

/*
 * Function: loadBedStatus
 * Purpose : Reads bed status records from data/beds_status.txt during startup
 *           and updates ward available bed counters.
 * Input   : None
 * Returns : None
 */
void loadBedStatus() {
    FILE *fp = fopen(BED_FILE, "r");
    if (fp == NULL) {
        return; /* File does not exist yet */
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

            /* Decrement available ward bed counter if bed is occupied */
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
    printf("[File Storage] Successfully loaded bed matrix from the database.\n");
}
