#include <stdio.h>
#include <string.h>
#include "billing.h"
#include "patient.h"
#include "ward.h"
#include "bed.h"
#include "ui_effects.h"

float calculateWardCost(float dailyRate, int days) {
    if (days <= 0) {
        return 0.0f;
    }
    return dailyRate * (float)days;
}

float calculateEmergencySurcharge(float baseFee, int emergencyStatus) {
    if (emergencyStatus == 3) {
        return baseFee * 0.50f; // 50% Surcharge for Level 3 Critical
    } else if (emergencyStatus == 2) {
        return baseFee * 0.20f; // 20% Surcharge for Level 2 Urgent (Fixed per specification)
    }
    return 0.0f; // 0% Surcharge for Level 1 Normal
}

float calculateGrossTotal(float wardCost, float baseCharge) {
    return wardCost + baseCharge;
}

float calculateGrossTotalV4(float baseFee, float surcharge, float wardCost) {
    return baseFee + surcharge + wardCost;
}

float calculateAgeSubsidy(float grossTotal, int age) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15f; // 15% Subsidy Discount
    }
    return 0.0f;
}

float calculateFinalAmount(float grossTotal) {
    return grossTotal;
}

float calculateFinalAmountV4(float grossTotal, float discount) {
    return grossTotal - discount;
}

float calculateEstimatedWaitTime(int specialtyID, int emergencyStatus) {
    if (emergencyStatus == 3) {
        return 0.0f; // Level 3 Critical case gets immediate attention (0 wait time)
    }

    int avgTime = 15;
    if (specialtyID >= 1 && specialtyID <= MAX_SPECIALTIES) {
        avgTime = specialties[specialtyID - 1].consultationTime;
    }

    // Count non-critical patients in queue for this specialty
    int queueCount = 0;
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].emergencyStatus != 3 && patients[i].specialtyID == specialtyID) {
            queueCount++;
        }
    }
    // If no specialty assigned, fallback to general non-critical queue count
    if (queueCount == 0) {
        for (int i = 0; i < patientCount; i++) {
            if (patients[i].emergencyStatus != 3) {
                queueCount++;
            }
        }
    }

    return (float)(queueCount * avgTime);
}

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

    // Perform Financial Calculations
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

    printf("\n========================================================================\n");
    printf("               SMART HOSPITAL ADMISSION & BILL                          \n");
    printf("========================================================================\n");
    printf(" Patient ID:              %s\n", p->id);
    printf(" Patient Name:            %s\n", p->name);
    printf(" Age:                     %d Years %s\n", p->age, isSubsidyEligible ? "(15% Subsidy Eligible)" : "(Standard Rate)");
    printf(" Specialty:               %s\n", specialties[specialtyID - 1].name);
    
    if (p->wardID > 0) {
        printf(" Assigned Ward:           %s (Bed #%d)\n", assignedWard ? assignedWard->name : "Admitted", p->bedID);
    } else {
        printf(" Assigned Ward:           Outpatient (OPD - No Bed Allocated)\n");
    }

    printf(" Urgency Level:           %s\n", urgencyStr);
    printf("------------------------------------------------------------------------\n");
    printf(" Base Consultation Fee:   LKR %10.2f\n", baseFee);
    printf(" Emergency Surcharge:     LKR %10.2f (%s)\n", surcharge, surchargePctStr);
    printf(" Ward Stay Cost (%d Days): LKR %10.2f\n", daysAdmitted, wardCost);
    printf("------------------------------------------------------------------------\n");
    printf(" Gross Total Bill:        LKR %10.2f\n", grossTotal);
    printf(" Age Subsidy Discount:    LKR -%9.2f %s\n", discount, isSubsidyEligible ? "(15%)" : "(0%)");
    printf("------------------------------------------------------------------------\n");
    printf(" Final Payable Amount:    LKR %10.2f\n", finalAmount);
    if (p->emergencyStatus == 3) {
        printf(" Estimated Waiting Time:  0.00 mins (Immediate Attention)\n");
    } else {
        printf(" Estimated Waiting Time:  %.2f mins\n", waitTime);
    }
    printf("========================================================================\n");
}
