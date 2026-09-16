#ifndef BILLING_H
#define BILLING_H

float calculateWardCost(float dailyRate, int days);
float calculateGrossTotal(float wardCost, float baseCharge);
float calculateFinalAmount(float grossTotal);
void processBillCalculation();

#endif
