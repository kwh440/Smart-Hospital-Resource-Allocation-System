/*
 * Smart Hospital & Resource Allocation System
 *
 * File: billing.h
 * Purpose: Header file for medical billing calculations, emergency surcharge evaluation,
 *          age subsidy discount math, wait time estimation, and receipt processing.
 */

#ifndef BILLING_H
#define BILLING_H

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */

/* Calculates total ward stay cost based on daily ward rate and admitted days. */
float calculateWardCost(float dailyRate, int days);

/* Calculates emergency surcharge based on triage urgency level (Level 2 = 20%, Level 3 = 50%). */
float calculateEmergencySurcharge(float baseFee, int emergencyStatus);

/* Calculates gross total bill from ward cost and base charge. */
float calculateGrossTotal(float wardCost, float baseCharge);

/* Calculates gross total bill from base fee, emergency surcharge, and ward stay cost. */
float calculateGrossTotalV4(float baseFee, float surcharge, float wardCost);

/* Calculates 15% age subsidy discount for patients under age 5 or over age 65. */
float calculateAgeSubsidy(float grossTotal, int age);

/* Returns final payable amount. */
float calculateFinalAmount(float grossTotal);

/* Calculates final payable amount after subtracting age subsidy discount. */
float calculateFinalAmountV4(float grossTotal, float discount);

/* Calculates estimated consultation queue waiting time in minutes. */
float calculateEstimatedWaitTime(int specialtyID, int emergencyStatus);

/* Interactive bill calculation routine, receipt display, and text receipt exporting. */
void processBillCalculation();

#endif
