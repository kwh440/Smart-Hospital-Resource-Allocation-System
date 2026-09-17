#include <stdio.h>
#include <string.h>
#include "billing.h"
#include "patient.h"
#include "ward.h"
#include "bed.h"

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
        return baseFee * 0.25f; // 25% Surcharge for Level 2 Urgent
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

    // Standard consultation times per specialty (mins)
    int avgTime = 15;
    if (specialtyID == 2) avgTime = 20;      // Pediatrics
    else if (specialtyID == 3) avgTime = 30; // Cardiology
    else if (specialtyID == 4) avgTime = 30; // Neurology
    else if (specialtyID == 5) avgTime = 25; // Orthopedics

    // Count patients assigned to this specialty
    int queueCount = 0;
    for (int i = 0; i < patientCount; i++) {
        // Count non-critical patients in queue
        if (patients[i].emergencyStatus != 3) {
            queueCount++;
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

    Patient *p = findPatientByID(patientID);
    if (p == NULL) {
        printf("[Error] Patient with ID '%s' was not found.\n", patientID);
        return;
    }

    displaySpecialties();
    int specialtyID;
    printf("Select Doctor Specialty ID for consultation (1-5): ");
    if (scanf("%d", &specialtyID) != 1 || specialtyID < 1 || specialtyID > 5) {
        printf("[Note] Invalid specialty selection. Defaulting to Specialty #1 (General Medicine).\n");
        specialtyID = 1;
    }
    float baseFee = specialties[specialtyID - 1].baseFee;

    int daysAdmitted = 0;
    float dailyRate = 0.0f;
    Ward *assignedWard = NULL;

    if (p->wardID > 0) {
        assignedWard = getWardByID(p->wardID);
        if (assignedWard != NULL) {
            dailyRate = assignedWard->dailyRate;
        }
        printf("Enter number of days admitted in Ward #%d (%s): ", p->wardID, (assignedWard ? assignedWard->name : "Ward"));
        if (scanf("%d", &daysAdmitted) != 1 || daysAdmitted < 0) {
            daysAdmitted = 0;
        }
    }

    // Perform V4 Advanced Financial Calculations
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
                                  (p->emergencyStatus == 2) ? "25%" : "0%";

    printf("\n========================================================================\n");
    printf("               SMART HOSPITAL ADMISSION & BILL STATEMENT (V4)           \n");
    printf("========================================================================\n");
    printf(" Patient ID:              %s\n", p->id);
    printf(" Patient Name:            %s\n", p->name);
    printf(" Age:                     %d Years %s\n", p->age, isSubsidyEligible ? "(15% Subsidy Eligible)" : "(Standard Rate)");
    printf(" Specialty:               %s\n", specialties[specialtyID - 1].name);
    
    if (p->wardID > 0 && p->bedID > 0) {
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
    printf(" Age Subsidy Discount:    LKR %10.2f %s\n", -discount, isSubsidyEligible ? "(-15%)" : "(0%)");
    printf("------------------------------------------------------------------------\n");
    printf(" Final Payable Amount:    LKR %10.2f\n", finalAmount);
    if (p->emergencyStatus == 3) {
        printf(" Estimated Waiting Time:  %.2f mins (Immediate Attention)\n", waitTime);
    } else {
        printf(" Estimated Waiting Time:  %.2f mins\n", waitTime);
    }
    printf("========================================================================\n");
}
