#include <stdio.h>
#include "billing.h"
#include "patient.h"
#include "ward.h"

float calculateWardCost(float dailyRate, int days) {
    if (days <= 0) {
        return 0.0f;
    }
    return dailyRate * (float)days;
}

float calculateGrossTotal(float wardCost, float baseCharge) {
    return wardCost + baseCharge;
}

float calculateFinalAmount(float grossTotal) {
    return grossTotal;
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

    if (p->wardID > 0) {
        Ward *w = getWardByID(p->wardID);
        if (w != NULL) {
            dailyRate = w->dailyRate;
        }
        printf("Enter number of days admitted in Ward #%d (%s): ", p->wardID, (w ? w->name : "Ward"));
        if (scanf("%d", &daysAdmitted) != 1 || daysAdmitted < 0) {
            daysAdmitted = 0;
        }
    }

    float wardCost = calculateWardCost(dailyRate, daysAdmitted);
    float grossTotal = calculateGrossTotal(wardCost, baseFee);
    float finalAmount = calculateFinalAmount(grossTotal);

    printf("\n=========================================================\n");
    printf("            SMART HOSPITAL OFFICIAL BILL RECEIPT         \n");
    printf("=========================================================\n");
    printf(" Patient ID            : %s\n", p->id);
    printf(" Patient Name          : %s\n", p->name);
    printf(" Age                   : %d Years\n", p->age);
    printf(" Medical Specialty     : %s\n", specialties[specialtyID - 1].name);
    printf(" Doctor Consultation   : LKR %.2f\n", baseFee);
    printf(" Ward Stay (%d Days)    : LKR %.2f\n", daysAdmitted, wardCost);
    printf("---------------------------------------------------------\n");
    printf(" Gross Total Bill      : LKR %.2f\n", grossTotal);
    printf(" Final Payable Amount  : LKR %.2f\n", finalAmount);
    printf("=========================================================\n");
}
