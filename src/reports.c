/*
 * Smart Hospital & Resource Allocation System
 *
 * File: reports.c
 * Purpose: Aggregates system analytics, triage urgency distributions, financial revenue summaries,
 *          per-ward bed occupancy utilization percentages, and highest-paying patient identifying report.
 */

#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "billing.h"
#include "ui_effects.h"

/* ============================================================
   PERFORMANCE REPORT GENERATION
   ============================================================ */

/*
 * Function: generatePerformanceReport
 * Purpose : Generates a comprehensive hospital performance analytics report displaying:
 *           1. Patient triage distribution breakdown with colored block bar charts.
 *           2. Financial revenue analysis (Base Fees, Surcharges, Ward Costs, Discounts, Net Revenue).
 *           3. Per-ward bed occupancy utilization percentages and block progress charts.
 *           4. Identification of highest-paying registered patient.
 * Input   : None
 * Returns : None
 */
void generatePerformanceReport() {
    /* Calculate total occupied beds across all hospital wards */
    int totalOccupiedBeds = 0;
    for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
        if (beds[i].status == 1) totalOccupiedBeds++;
    }

    showLoadingSpinner("Generating Hospital Performance Analytics Report...", 3000);

    /* Render real-time hospital occupancy dashboard header */
    showHospitalStatusDisplay(patientCount, totalOccupiedBeds, TOTAL_BEDS_IN_HOSPITAL);

    printf("\n========================================================================\n");
    printf("         SMART HOSPITAL SYSTEM - PERFORMANCE & ANALYTICS REPORT         \n");
    printf("========================================================================\n");

    if (patientCount == 0) {
        printf("\n[Information] No patient data registered yet. Register patients to generate report.\n");
        printf("========================================================================\n");
        return;
    }

    /* -------------------------------------------------------------------------
       1. Patient Triage Breakdown (Urgency Levels)
       ------------------------------------------------------------------------- */
    int normalCount = 0, urgentCount = 0, criticalCount = 0;
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].emergencyStatus == 3) criticalCount++;
        else if (patients[i].emergencyStatus == 2) urgentCount++;
        else normalCount++;
    }

    float normalPct = ((float)normalCount / (float)patientCount) * 100.0f;
    float urgentPct = ((float)urgentCount / (float)patientCount) * 100.0f;
    float criticalPct = ((float)criticalCount / (float)patientCount) * 100.0f;

    printf("\n--- 1. PATIENT REGISTRATION & TRIAGE SUMMARY ---\n");
    printf(" Total Patients Registered : %d\n", patientCount);
    printf("   - Level 1 (Normal OPD)  : %d (%.1f%%)\n", normalCount, normalPct);
    printf("   - Level 2 (Urgent)      : %d (%.1f%%)\n", urgentCount, urgentPct);
    printf("   - Level 3 (Critical)    : %d (%.1f%%)\n", criticalCount, criticalPct);
    printf("\n [Triage Distribution Chart]\n");
    drawBarChart("Normal OPD", normalCount, patientCount, 20);
    drawBarChart("Urgent Priority", urgentCount, patientCount, 20);
    drawBarChart("Critical Emergency", criticalCount, patientCount, 20);

    /* -------------------------------------------------------------------------
       2. Financial Analytics & Revenue Summaries
       ------------------------------------------------------------------------- */
    float totalBaseFees = 0.0f;
    float totalSurcharges = 0.0f;
    float totalWardCosts = 0.0f;
    float totalGrossRevenue = 0.0f;
    float totalDiscounts = 0.0f;
    float totalNetRevenue = 0.0f;

    float maxBill = -1.0f;
    int maxPatientIndex = -1;

    for (int i = 0; i < patientCount; i++) {
        float baseFee = 1500.0f;
        if (patients[i].specialtyID >= 1 && patients[i].specialtyID <= MAX_SPECIALTIES) {
            baseFee = specialties[patients[i].specialtyID - 1].baseFee;
        }

        float dailyRate = 0.0f;
        if (patients[i].wardID > 0) {
            Ward *w = getWardByID(patients[i].wardID);
            if (w != NULL) {
                dailyRate = w->dailyRate;
            }
        }

        int stayDays = (patients[i].wardID > 0) ? patients[i].daysAdmitted : 0;

        float surcharge = calculateEmergencySurcharge(baseFee, patients[i].emergencyStatus);
        float wardCost = calculateWardCost(dailyRate, stayDays);
        float grossTotal = calculateGrossTotalV4(baseFee, surcharge, wardCost);
        float discount = calculateAgeSubsidy(grossTotal, patients[i].age);
        float finalBill = calculateFinalAmountV4(grossTotal, discount);

        totalBaseFees += baseFee;
        totalSurcharges += surcharge;
        totalWardCosts += wardCost;
        totalGrossRevenue += grossTotal;
        totalDiscounts += discount;
        totalNetRevenue += finalBill;

        if (finalBill > maxBill) {
            maxBill = finalBill;
            maxPatientIndex = i;
        }
    }

    printf("\n--- 2. FINANCIAL REVENUE & DISCOUNT SUMMARY ---\n");
    printf(" Total Base Consultation Fees : LKR %12.2f\n", totalBaseFees);
    printf(" Total Emergency Surcharges    : LKR %12.2f\n", totalSurcharges);
    printf(" Total Ward Stay Costs        : LKR %12.2f\n", totalWardCosts);
    printf("----------------------------------------------------\n");
    printf(" TOTAL GROSS REVENUE          : LKR %12.2f\n", totalGrossRevenue);
    printf(" TOTAL DISCOUNTS GRANTED      : LKR %12.2f (-15%% Age Subsidy)\n", -totalDiscounts);
    printf("----------------------------------------------------\n");
    printf(" TOTAL NET REVENUE EARNED     : LKR %12.2f\n", totalNetRevenue);

    /* -------------------------------------------------------------------------
       3. Bed Occupancy Percentage Report Per Ward
       ------------------------------------------------------------------------- */
    printf("\n--- 3. BED OCCUPANCY & UTILIZATION METRICS ---\n");
    printf("%-8s %-20s %-12s %-12s %-12s %-12s\n",
           "Ward ID", "Ward Name", "Total Beds", "Occupied", "Available", "Occupancy %");
    printf("-------------------------------------------------------------------------\n");

    int grandTotal = 0, grandOccupied = 0, grandAvailable = 0;

    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;

        int occupiedCount = 0;
        for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
            if (beds[i].wardID == w && beds[i].status == 1) {
                occupiedCount++;
            }
        }

        int availCount = ward->totalBeds - occupiedCount;
        float occPct = (ward->totalBeds > 0) ? ((float)occupiedCount / (float)ward->totalBeds) * 100.0f : 0.0f;

        grandTotal += ward->totalBeds;
        grandOccupied += occupiedCount;
        grandAvailable += availCount;

        printf("%-8d %-20s %-12d %-12d %-12d %-11.1f%%\n",
               ward->wardID, ward->name, ward->totalBeds, occupiedCount, availCount, occPct);
    }

    float grandPct = (grandTotal > 0) ? ((float)grandOccupied / (float)grandTotal) * 100.0f : 0.0f;
    printf("-------------------------------------------------------------------------\n");
    printf("%-8s %-20s %-12d %-12d %-12d %-11.1f%%\n",
           "TOTAL", "All Wards Combined", grandTotal, grandOccupied, grandAvailable, grandPct);

    printf("\n [Ward Occupancy Utilization Chart]\n");
    for (int w = 1; w <= MAX_WARDS; w++) {
        Ward *ward = getWardByID(w);
        if (ward == NULL) continue;
        int occupiedCount = 0;
        for (int i = 0; i < TOTAL_BEDS_IN_HOSPITAL; i++) {
            if (beds[i].wardID == w && beds[i].status == 1) {
                occupiedCount++;
            }
        }
        drawBarChart(ward->name, occupiedCount, ward->totalBeds, 20);
    }

    /* -------------------------------------------------------------------------
       4. Highest-Paying Patient Summary
       ------------------------------------------------------------------------- */
    printf("\n--- 4. HIGHEST-PAYING PATIENT RECORD ---\n");
    if (maxPatientIndex != -1) {
        Patient topP = patients[maxPatientIndex];
        Ward *w = (topP.wardID > 0) ? getWardByID(topP.wardID) : NULL;
        printf(" Patient ID          : %s\n", topP.id);
        printf(" Patient Name        : %s\n", topP.name);
        printf(" Age                 : %d Years\n", topP.age);
        printf(" Urgency Level       : %s\n", (topP.emergencyStatus == 3) ? "Critical" : (topP.emergencyStatus == 2) ? "Urgent" : "Normal");
        printf(" Ward Admission      : %s\n", w ? w->name : "Outpatient (OPD)");
        printf(" Highest Total Bill  : LKR %.2f\n", maxBill);
    } else {
        printf(" No bill records available.\n");
    }
    printf("========================================================================\n");
}
