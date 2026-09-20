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

    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║               SMART HOSPITAL SYSTEM - PERFORMANCE & ANALYTICS REPORT             ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);

    if (patientCount == 0) {
        printf("\n%s[Information] No patient data registered yet. Register patients to generate report.%s\n", COLOR_CYAN, COLOR_RESET);
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

    char buf1[80], buf2[80], buf3[80];
    snprintf(buf1, sizeof(buf1), "%d (%.1f%%)", normalCount, normalPct);
    snprintf(buf2, sizeof(buf2), "%d (%.1f%%)", urgentCount, urgentPct);
    snprintf(buf3, sizeof(buf3), "%d (%.1f%%)", criticalCount, criticalPct);

    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                       1. PATIENT REGISTRATION & TRIAGE SUMMARY                   ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Total Patients Registered : %-54d %s║%s\n", COLOR_CYAN, COLOR_RESET, patientCount, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s   - Level 1 (Normal OPD)  : %s%-55s%s %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_GREEN, buf1, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s   - Level 2 (Urgent)      : %s%-55s%s %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_YELLOW, buf2, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s   - Level 3 (Critical)    : %s%-55s%s %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_RED, buf3, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);

    printf("\n%s[Triage Distribution Chart]%s\n", COLOR_CYAN, COLOR_RESET);
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

    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                       2. FINANCIAL REVENUE & DISCOUNT SUMMARY                    ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Total Base Consultation Fees : LKR %12.2f                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, totalBaseFees, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Total Emergency Surcharges   : LKR %12.2f                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, totalSurcharges, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Total Ward Stay Costs        : LKR %12.2f                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, totalWardCosts, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  TOTAL GROSS REVENUE          : LKR %12.2f                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, totalGrossRevenue, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  TOTAL DISCOUNTS GRANTED      : %sLKR %12.2f%s (-15%% Age Subsidy)                %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_YELLOW, -totalDiscounts, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  TOTAL NET REVENUE EARNED     : %sLKR %12.2f%s                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_GREEN, totalNetRevenue, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);

    /* -------------------------------------------------------------------------
       3. Bed Occupancy Percentage Report Per Ward
       ------------------------------------------------------------------------- */
    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                 3. BED OCCUPANCY & UTILIZATION METRICS SUMMARY                   ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠═════════╦══════════════════════╦════════════╦════════════╦═══════════╦═════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s Ward ID  %s║%s Ward Name            %s║%s Total Beds %s║%s Occupied   %s║%s Available %s║%s Occupancy %%  %s║%s\n",
           COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s╠═════════╬══════════════════════╬════════════╬════════════╬═══════════╬═════════════╣%s\n", COLOR_CYAN, COLOR_RESET);

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

        printf("%s║%s %-7d %s║%s %-20s %s║%s %-10d %s║%s %-10d %s║%s %-9d %s║%s %-11.1f%% %s║%s\n",
               COLOR_CYAN, COLOR_RESET,
               ward->wardID,
               COLOR_CYAN, COLOR_RESET,
               ward->name,
               COLOR_CYAN, COLOR_RESET,
               ward->totalBeds,
               COLOR_CYAN, COLOR_RESET,
               occupiedCount,
               COLOR_CYAN, COLOR_RESET,
               availCount,
               COLOR_CYAN, COLOR_RESET,
               occPct,
               COLOR_CYAN, COLOR_RESET);
    }

    float grandPct = (grandTotal > 0) ? ((float)grandOccupied / (float)grandTotal) * 100.0f : 0.0f;
    printf("%s╠═════════╬══════════════════════╬════════════╬════════════╬═══════════╬═════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s %-7s %s║%s %-20s %s║%s %-10d %s║%s %-10d %s║%s %-9d %s║%s %-11.1f%% %s║%s\n",
           COLOR_CYAN, COLOR_RESET,
           "TOTAL",
           COLOR_CYAN, COLOR_RESET,
           "All Wards Combined",
           COLOR_CYAN, COLOR_RESET,
           grandTotal,
           COLOR_CYAN, COLOR_RESET,
           grandOccupied,
           COLOR_CYAN, COLOR_RESET,
           grandAvailable,
           COLOR_CYAN, COLOR_RESET,
           grandPct,
           COLOR_CYAN, COLOR_RESET);
    printf("%s╚═════════╩══════════════════════╩════════════╩════════════╩═══════════╩═════════════╝%s\n", COLOR_CYAN, COLOR_RESET);

    printf("\n%s[Ward Occupancy Utilization Chart]%s\n", COLOR_CYAN, COLOR_RESET);
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
    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                  4. HIGHEST-PAYING PATIENT RECORD                                  ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    if (maxPatientIndex != -1) {
        Patient topP = patients[maxPatientIndex];
        Ward *w = (topP.wardID > 0) ? getWardByID(topP.wardID) : NULL;
        const char *urgencyStr = (topP.emergencyStatus == 3) ? "Critical" : (topP.emergencyStatus == 2) ? "Urgent" : "Normal";
        const char *wardStr = w ? w->name : "Outpatient (OPD)";

        printf("%s║%s  Patient ID          : %-55s     %s║%s\n", COLOR_CYAN, COLOR_RESET, topP.id, COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Patient Name        : %-55s     %s║%s\n", COLOR_CYAN, COLOR_RESET, topP.name, COLOR_CYAN, COLOR_RESET);

        char ageBuf[30];
        snprintf(ageBuf, sizeof(ageBuf), "%d Years", topP.age);
        printf("%s║%s  Age                 : %-55s     %s║%s\n", COLOR_CYAN, COLOR_RESET, ageBuf, COLOR_CYAN, COLOR_RESET);

        printf("%s║%s  Urgency Level       : %-55s     %s║%s\n", COLOR_CYAN, COLOR_RESET, urgencyStr, COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Ward Admission      : %-55s     %s║%s\n", COLOR_CYAN, COLOR_RESET, wardStr, COLOR_CYAN, COLOR_RESET);

        char billBuf[50];
        snprintf(billBuf, sizeof(billBuf), "LKR %.2f", maxBill);
        printf("%s║%s  Highest Total Bill  : %s%-55s%s     %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_GREEN, billBuf, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    } else {
        printf("%s║%s  No bill records available.                                                      %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    }
    printf("%s╚════════════════════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
}
