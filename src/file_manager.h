/*
 * Smart Hospital & Resource Allocation System
 *
 * File: file_manager.h
 * Purpose: Header file for data persistence functions managing save and load operations
 *          for patient records (data/patient_records.txt) and bed status (data/beds_status.txt).
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */

/* Initializes target data directory and persistent storage files. */
void initDirectoriesAndFiles();

/* Saves all active patient records to data/patient_records.txt. */
void savePatientRecords();

/* Loads previously saved patient records from data/patient_records.txt upon startup. */
void loadPatientRecords();

/* Saves current bed matrix status to data/beds_status.txt. */
void saveBedStatus();

/* Loads bed occupancy statuses from data/beds_status.txt upon startup. */
void loadBedStatus();

#endif
