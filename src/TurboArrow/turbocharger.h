#pragma once

#ifndef TURBOCHARGER
#define TURBOCHARGER

#include "common.h"

class Turbocharger
{
public:
    static double getTargetFuelAirRatio(FLOAT64 ambientTemperature, FLOAT64 ambientPressure, double mixturePercentage) {
        double maxPowerManifoldPressure = 41.0;
        double criticalAltitude = 12000.0;
        double icaoStandardPressureAtCriticalAltitude = 19.03455;
        double maxTurboBoostPressure = maxPowerManifoldPressure - icaoStandardPressureAtCriticalAltitude;
        double fullRichSpecificVolumeConstant = 0.110236;
        double turboBoostCorrectionFactor = 1.252303;
        double turboSpecificVolumeConstant = fullRichSpecificVolumeConstant * turboBoostCorrectionFactor;
        double slope_90_100 = 0.000487531;
        double slope_75_90 = 0.000243697;
        double slope_60_75 = 0.000409167;
        double slope_20_60 = 0.000954534;
        double ambientPressurePa = ambientPressure * 3386.39;
        double maxManifoldPressurePa = maxPowerManifoldPressure * 3386.39;
        double ambientTemperatureK = ambientTemperature + 273.15;
        double maxManifoldDensity = 0.0;
        if (ambientPressure >= icaoStandardPressureAtCriticalAltitude) {
            maxManifoldDensity = maxManifoldPressurePa / (287.058 * pow(pow(ambientPressurePa, (1.0 - 1.4)) * pow(ambientTemperatureK, 1.4) / pow(maxManifoldPressurePa, (1.0 - 1.4)), (1.0 / 1.40)));
        }
        else {
            maxManifoldDensity = ((ambientPressure + maxTurboBoostPressure) * 3386.39) / (287.058 * pow(pow(ambientPressurePa, (1.0 - 1.4)) * pow(ambientTemperatureK, 1.4) / pow(((ambientPressure + maxTurboBoostPressure) * 3386.39), (1.0 - 1.4)), (1.0 / 1.40)));
        }
        double fullRichSpecificVolume = 1.0 / maxManifoldDensity;
        if (mixturePercentage == 100.0) {
            double targetFuelAirRatio = fullRichSpecificVolume * turboSpecificVolumeConstant;
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 90.0) {
            double targetFuelAirRatio = fullRichSpecificVolume * (turboSpecificVolumeConstant + (mixturePercentage - 100.0) * slope_90_100);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 75.0) {
            double targetFuelAirRatio = fullRichSpecificVolume * (turboSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (mixturePercentage - 90.0) * slope_75_90);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 60.0) {
            double targetFuelAirRatio = fullRichSpecificVolume * (turboSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (mixturePercentage - 75.0) * slope_60_75);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 20.0) {
            double targetFuelAirRatio = fullRichSpecificVolume * (turboSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (mixturePercentage - 60.0) * slope_20_60);
            return targetFuelAirRatio;
        }
        else {
            double slope_0_20 = (turboSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (20.0 - 60.0) * slope_20_60) / 20.0;
            double targetFuelAirRatio = fullRichSpecificVolume * (turboSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (20.0 - 60.0) * slope_20_60 + (mixturePercentage - 20.0) * slope_0_20);
            return targetFuelAirRatio;
        }
    }
};

#endif // !TURBOCHARGER
