/*
 * Smart Hospital & Resource Allocation System
 *
 * File: billing.c
 * Purpose: Implementation of medical billing formulas, emergency surcharges,
 *          age subsidy discounts, queue wait-time calculations, and itemized receipt exporting.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "ui_effects.h"

/* ============================================================
   BILLING FORMULA CALCULATIONS
   ============================================================ */

/*
 * Function: calculateWardCost
 * Purpose : Calculates total ward stay cost based on daily ward rate and admitted days.
 * Input   : dailyRate - Daily accommodation rate of assigned ward.
 *           days      - Number of admitted stay days.
 * Returns : Total ward stay cost in LKR (0.0f if days <= 0).
 */
float calculateWardCost(float dailyRate, int days) {
    if (days <= 0) {
        return 0.0f;
    }
    return dailyRate * (float)days;
}

/*
 * Function: calculateEmergencySurcharge
 * Purpose : Calculates emergency triage surcharge based on urgency level:
 *           - Urgency Level 1 (Normal OPD) : 0% Surcharge
 *           - Urgency Level 2 (Urgent)     : 20% of Base Consultation Fee
 *           - Urgency Level 3 (Critical)   : 50% of Base Consultation Fee
 * Input   : baseFee         - Specialty base consultation fee.
 *           emergencyStatus - Patient triage urgency level (1, 2, 3).
 * Returns : Emergency surcharge amount in LKR.
 */
float calculateEmergencySurcharge(float baseFee, int emergencyStatus) {
    if (emergencyStatus == 3) {
        return baseFee * 0.50f; /* 50% Surcharge for Level 3 Critical emergency */
    } else if (emergencyStatus == 2) {
        return baseFee * 0.20f; /* 20% Surcharge for Level 2 Urgent case */
    }
    return 0.0f; /* 0% Surcharge for Level 1 Normal OPD */
}

/*
 * Function: calculateGrossTotal
 * Purpose : Calculates basic gross bill total from ward cost and base charge.
 * Input   : wardCost   - Total ward stay cost.
 *           baseCharge - Base consultation charge.
 * Returns : Sum of ward cost and base charge.
 */
float calculateGrossTotal(float wardCost, float baseCharge) {
    return wardCost + baseCharge;
}

/*
 * Function: calculateGrossTotalV4
 * Purpose : Calculates complete gross total bill:
 *           Gross Total = Base Consultation Fee + Emergency Surcharge + Ward Stay Cost
 * Input   : baseFee   - Base doctor consultation fee.
 *           surcharge - Emergency surcharge amount.
 *           wardCost  - Total ward stay cost.
 * Returns : Gross total bill in LKR.
 */
float calculateGrossTotalV4(float baseFee, float surcharge, float wardCost) {
    return baseFee + surcharge + wardCost;
}

/*
 * Function: calculateAgeSubsidy
 * Purpose : Applies a 15% age subsidy discount on Gross Total Bill for vulnerable age groups:
 *           - Age < 5 years (Paediatric subsidy)
 *           - Age > 65 years (Senior citizen subsidy)
 * Input   : grossTotal - Gross total bill amount.
 *           age        - Patient age in years.
 * Returns : Discount amount in LKR (0.0f if not eligible).
 */
float calculateAgeSubsidy(float grossTotal, int age) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15f; /* 15% Subsidy Discount */
    }
    return 0.0f;
}

/*
 * Function: calculateFinalAmount
 * Purpose : Returns gross total as final amount without discount.
 * Input   : grossTotal - Gross total bill amount.
 * Returns : Final amount.
 */
float calculateFinalAmount(float grossTotal) {
    return grossTotal;
}

/*
 * Function: calculateFinalAmountV4
 * Purpose : Calculates final payable bill amount:
 *           Final Amount = Gross Total - Age Subsidy Discount
 * Input   : grossTotal - Gross total bill amount.
 *           discount   - Age subsidy discount amount.
 * Returns : Final payable amount in LKR.
 */
float calculateFinalAmountV4(float grossTotal, float discount) {
    return grossTotal - discount;
}

/* ============================================================
   WAITING TIME ESTIMATION
   ============================================================ */

/*
 * Function: calculateEstimatedWaitTime
 * Purpose : Estimates consultation queue wait time based on non-critical queue size
 *           and specialty consultation duration:
 *           - Level 3 Critical emergency cases receive 0.00 mins (Immediate Attention).
 *           - Other cases: Wait Time = Queue Count * Specialty Consultation Time.
 * Input   : specialtyID     - Selected specialty ID (1 to 7).
 *           emergencyStatus - Patient urgency level (1, 2, 3).
 * Returns : Estimated waiting time in minutes.
 */
float calculateEstimatedWaitTime(int specialtyID, int emergencyStatus) {
    if (emergencyStatus == 3) {
        return 0.0f; /* Critical emergency cases get immediate attention */
    }

    int avgTime = 15;
    if (specialtyID >= 1 && specialtyID <= MAX_SPECIALTIES) {
        avgTime = specialties[specialtyID - 1].consultationTime;
    }

    /* Count non-critical patients in queue for this specialty */
    int queueCount = 0;
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].emergencyStatus != 3 && patients[i].specialtyID == specialtyID) {
            queueCount++;
        }
    }
    /* Fallback to general queue count if specific specialty queue count is empty */
    if (queueCount == 0) {
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].emergencyStatus != 3) {
                queueCount++;
            }
        }
    }

    return (float)(queueCount * avgTime);
}

/* ============================================================
   BILLING PROCESS & RECEIPT EXPORTING
   ============================================================ */

/*
 * Function: processBillCalculation
 * Purpose : Interactively calculates patient admission and consultation bills,
 *           prompts for days admitted verification, prints itemized receipt to screen,
 *           and exports permanent text receipt file to data/receipts/ directory.
 * Input   : None
 * Returns : None
 */
void processBillCalculation() {
    if (patientCount == 0) {
        printf("\n[Error] No registered patients found. Please register a patient first.\n");
        return;
    }

    char patientID[15];
    printf("\nEnter Patient ID for bill calculation (e.g., PAT-1001): ");
    scanf("%14s", patientID);
    clearInputBuffer();

    Patient *p = findPatientByID(patientID);
    if (p == NULL) {
        printf("[Error] Patient with ID '%s' was not found.\n", patientID);
        return;
    }

    int specialtyID = p->specialtyID;
    if (specialtyID < 1 || specialtyID > MAX_SPECIALTIES) {
        displaySpecialties();
        specialtyID = readIntBounded("Select Doctor Specialty ID for consultation (1-7): ", 1, MAX_SPECIALTIES);
        p->specialtyID = specialtyID;
    }
    float baseFee = specialties[specialtyID - 1].baseFee;

    int daysAdmitted = p->daysAdmitted;
    float dailyRate = 0.0f;
    Ward *assignedWard = NULL;

    /* If admitted to ward, prompt to verify or update stay duration */
    if (p->wardID > 0) {
        assignedWard = getWardByID(p->wardID);
        if (assignedWard != NULL) {
            dailyRate = assignedWard->dailyRate;
        }

        printf("\n[Verification Prompt] Is Days Admitted correct (%d days)? (1 = Yes, 0 = Update): ", p->daysAdmitted);
        int confirmDays;
        if (scanf("%d", &confirmDays) == 1 && confirmDays == 0) {
            daysAdmitted = readIntBounded("Enter updated/correct Days Admitted: ", 0, 365);
            p->daysAdmitted = daysAdmitted;
            printf("[Update] Days Admitted updated to %d days.\n", p->daysAdmitted);
        } else {
            clearInputBuffer();
        }
    }

    /* Compute financial billing breakdown */
    float surcharge = calculateEmergencySurcharge(baseFee, p->emergencyStatus);
    float wardCost = calculateWardCost(dailyRate, daysAdmitted);
    float grossTotal = calculateGrossTotalV4(baseFee, surcharge, wardCost);
    float discount = calculateAgeSubsidy(grossTotal, p->age);
    float finalAmount = calculateFinalAmountV4(grossTotal, discount);
    float waitTime = calculateEstimatedWaitTime(specialtyID, p->emergencyStatus);

    int isSubsidyEligible = (p->age < 5 || p->age > 65);
    const char *urgencyStr = (p->emergencyStatus == 3) ? "Level 3 (Critical)" :
                             (p->emergencyStatus == 2) ? "Level 2 (Urgent)" : "Level 1 (Normal)";
    const char *surchargePctStr = (p->emergencyStatus == 3) ? "50%" :
                                  (p->emergencyStatus == 2) ? "20%" : "0%";

    showLoadingSpinner("Calculating billing breakdown & subsidies...", 3000);

    char ageStr[60];
    snprintf(ageStr, sizeof(ageStr), "%d Years %s", p->age, isSubsidyEligible ? "(15%% Subsidy Eligible)" : "(Standard Rate)");

    char wardStr[80];
    if (p->wardID > 0) {
        snprintf(wardStr, sizeof(wardStr), "%s (Bed #%d)", assignedWard ? assignedWard->name : "Admitted", p->bedID);
    } else {
        snprintf(wardStr, sizeof(wardStr), "Outpatient (OPD - No Bed Allocated)");
    }

    char surStr[20];
    snprintf(surStr, sizeof(surStr), "(%s)", surchargePctStr);

    char wardCostLabel[40];
    snprintf(wardCostLabel, sizeof(wardCostLabel), "Ward Stay Cost (%d Days):", daysAdmitted);

    char waitStr[60];
    if (p->emergencyStatus == 3) {
        snprintf(waitStr, sizeof(waitStr), "0.00 mins (Immediate Attention)");
    } else {
        snprintf(waitStr, sizeof(waitStr), "%.2f mins", waitTime);
    }

    /* Render itemized bill statement to console */
    printf("\n%s╔════════════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║                     SMART HOSPITAL ADMISSION & BILL                                ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Patient ID:              %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, p->id, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Patient Name:            %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, p->name, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Age:                     %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, ageStr, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Specialty:               %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, specialties[specialtyID - 1].name, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Assigned Ward:           %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, wardStr, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Urgency Level:           %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, urgencyStr, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Base Consultation Fee:   LKR %10.2f                                           %s║%s\n", COLOR_CYAN, COLOR_RESET, baseFee, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Emergency Surcharge:     LKR %10.2f %-7s                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, surcharge, surStr, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  %-26s LKR %10.2f                                         %s║%s\n", COLOR_CYAN, COLOR_RESET, wardCostLabel, wardCost, COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Gross Total Bill:        LKR %10.2f                                           %s║%s\n", COLOR_CYAN, COLOR_RESET, grossTotal, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Age Subsidy Discount:    %sLKR -%9.2f%s %-7s                                   %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_YELLOW, discount, COLOR_RESET, isSubsidyEligible ? "(15%)" : "(0%)", COLOR_CYAN, COLOR_RESET);
    printf("%s╠════════════════════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Final Payable Amount:    %sLKR %10.2f%s                                           %s║%s\n", COLOR_CYAN, COLOR_RESET, COLOR_GREEN, finalAmount, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
    printf("%s║%s  Estimated Waiting Time:  %-52s     %s║%s\n", COLOR_CYAN, COLOR_RESET, waitStr, COLOR_CYAN, COLOR_RESET);
    printf("%s╚════════════════════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);

    /* Export billing receipt text file to data/receipts/ directory */
    #ifdef _WIN32
    system("mkdir data\\receipts 2> NUL");
    #else
    system("mkdir -p data/receipts 2>/dev/null");
    #endif

    char filename[64];
    snprintf(filename, sizeof(filename), "data/receipts/%s_receipt.txt", p->id);
    FILE *rf = fopen(filename, "w");
    if (rf != NULL) {
        fprintf(rf, "========================================================================\n");
        fprintf(rf, "               SMART HOSPITAL ADMISSION & BILL                          \n");
        fprintf(rf, "========================================================================\n");
        fprintf(rf, " Patient ID:              %s\n", p->id);
        fprintf(rf, " Patient Name:            %s\n", p->name);
        fprintf(rf, " Age:                     %d Years %s\n", p->age, isSubsidyEligible ? "(15%% Subsidy Eligible)" : "(Standard Rate)");
        fprintf(rf, " Specialty:               %s\n", specialties[specialtyID - 1].name);
        if (p->wardID > 0) {
            fprintf(rf, " Assigned Ward:           %s (Bed #%d)\n", assignedWard ? assignedWard->name : "Admitted", p->bedID);
        } else {
            fprintf(rf, " Assigned Ward:           Outpatient (OPD - No Bed Allocated)\n");
        }
        fprintf(rf, " Urgency Level:           %s\n", urgencyStr);
        fprintf(rf, "------------------------------------------------------------------------\n");
        fprintf(rf, " Base Consultation Fee:   LKR %10.2f\n", baseFee);
        fprintf(rf, " Emergency Surcharge:     LKR %10.2f (%s)\n", surcharge, surchargePctStr);
        fprintf(rf, " Ward Stay Cost (%d Days): LKR %10.2f\n", daysAdmitted, wardCost);
        fprintf(rf, "------------------------------------------------------------------------\n");
        fprintf(rf, " Gross Total Bill:        LKR %10.2f\n", grossTotal);
        fprintf(rf, " Age Subsidy Discount:    LKR -%9.2f %s\n", discount, isSubsidyEligible ? "(15%%)" : "(0%%)");
        fprintf(rf, "------------------------------------------------------------------------\n");
        fprintf(rf, " Final Payable Amount:    LKR %10.2f\n", finalAmount);
        if (p->emergencyStatus == 3) {
            fprintf(rf, " Estimated Waiting Time:  0.00 mins (Immediate Attention)\n");
        } else {
            fprintf(rf, " Estimated Waiting Time:  %.2f mins\n", waitTime);
        }
        fprintf(rf, "========================================================================\n");
        fclose(rf);
        printf("\n%s[Receipt Export]%s Itemized bill saved to the database.%s\n", COLOR_CYAN, COLOR_GREEN, COLOR_RESET);
    }
}
