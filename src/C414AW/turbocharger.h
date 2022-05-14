#pragma once

#ifndef TURBOCHARGER
#define TURBOCHARGER

#include "common.h"

class Turbocharger
{
public:
    static double getTargetFuelAirRatio(double mixturePercentage) {
        // The proportionality constant between Fuel/Air mixture and ambient specific volume measured in the sim:
        double fullRichSpecificVolumeConstant = 0.110236;
        double seaLevelSpecificVolume = 1.0 / 1.225;
        // Slope of the relationship between mixture lever position (in percent) and the ratio of fuel/air mixture to ambient specific volume -- measured from sim data.
        double slope_90_100 = 0.000487531;
        double slope_75_90 = 0.000243697;
        double slope_60_75 = 0.000409167;
        double slope_20_60 = 0.000954534;

        if (mixturePercentage == 100.0) {
            double targetFuelAirRatio = seaLevelSpecificVolume * fullRichSpecificVolumeConstant;
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 90.0) {
            double targetFuelAirRatio = seaLevelSpecificVolume * (fullRichSpecificVolumeConstant + (mixturePercentage - 100.0) * slope_90_100);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 75.0) {
            double targetFuelAirRatio = seaLevelSpecificVolume * (fullRichSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (mixturePercentage - 90.0) * slope_75_90);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 60.0) {
            double targetFuelAirRatio = seaLevelSpecificVolume * (fullRichSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (mixturePercentage - 75.0) * slope_60_75);
            return targetFuelAirRatio;
        }
        else if (mixturePercentage >= 20.0) {
            double targetFuelAirRatio = seaLevelSpecificVolume * (fullRichSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (mixturePercentage - 60.0) * slope_20_60);
            return targetFuelAirRatio;
        }
        else {
            double slope_0_20 = (fullRichSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (20.0 - 60.0) * slope_20_60) / 20.0;
            double targetFuelAirRatio = seaLevelSpecificVolume * (fullRichSpecificVolumeConstant + (90.0 - 100.0) * slope_90_100 + (75.0 - 90.0) * slope_75_90 + (60.0 - 75.0) * slope_60_75 + (20.0 - 60.0) * slope_20_60 + (mixturePercentage - 20.0) * slope_0_20);
            return targetFuelAirRatio;
        }
    }
};

#endif // !TURBOCHARGER
