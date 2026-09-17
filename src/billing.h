#ifndef BILLING_H
#define BILLING_H

float calculateWardCost(float dailyRate, int days);
float calculateEmergencySurcharge(float baseFee, int emergencyStatus);
float calculateGrossTotal(float wardCost, float baseCharge);
float calculateGrossTotalV4(float baseFee, float surcharge, float wardCost);
float calculateAgeSubsidy(float grossTotal, int age);
float calculateFinalAmount(float grossTotal);
float calculateFinalAmountV4(float grossTotal, float discount);
float calculateEstimatedWaitTime(int specialtyID, int emergencyStatus);
void processBillCalculation();

#endif
