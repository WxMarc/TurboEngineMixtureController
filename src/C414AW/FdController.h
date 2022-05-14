#pragma once

#ifndef FDCTRL
#define FDCTRL

#include "common.h"
#include "PidController.h"

class FdController
{
private:
    SimVars* simVars;

    /// <summary>
    /// An instance of the mixture PID controller.
    /// </summary>
    PidController* mixtureController[2];

    /// <summary>
    /// The current mixture control axis, from -16384 to 16384.
    /// </summary>  
    int mixtureAxis[2] = { 16384, 16384 };

    bool enabled = true;

    /// <summary>
    /// Calculates and updates mixture according to target and PIDs.
    /// </summary>
    /// <param name="deltaTime"></param>
    void updateMixture(double deltaTime) {
        EngineControlData controls;
        controls.mixtureLeft = this->getDesiredMixture(0, deltaTime);
        controls.mixtureRight = this->getDesiredMixture(1, deltaTime);
        SimConnect_SetDataOnSimObject(hSimConnect, DataTypes::EngineControls, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(EngineControlData), &controls);
    }

    /// <summary>
    /// Gets the mixture lever position to achieve the desired fuel/air mixture ratio
    /// </summary>
    /// <returns>Desired mixture setting</returns>
    double getDesiredMixture(int idx, double deltaTime) {
        double mixtureLeverPerc = 100.0 * (this->mixtureAxis[idx] + 16384) / 32768.0;

        if (!enabled) {
            return mixtureLeverPerc;
        }

        double targetFuelAirMixture = Turbocharger::getTargetFuelAirRatio(mixtureLeverPerc);
        double simFuelAirMixture = this->simVars->getFuelAirRatio(idx + 1);
        //printf("Target Mixture: %1.3f\nSim Mixture: %1.3f\n", targetFuelAirMixture, simFuelAirMixture);

        double error = targetFuelAirMixture - simFuelAirMixture;
        double pidOut = 0.0;
        pidOut = this->mixtureController[idx]->GetOutput(error, deltaTime);
        //printf("Output from PID Controller: %2.3f\n", error);
        return max(0, min(100, this->simVars->getMixtureLeverPosition(idx + 1) + pidOut));
    }

    void updateVisibleMixture(int idx) {
        int targetMixture = this->mixtureAxis[idx];
        double mixtureLeverPerc = 100.0 * (targetMixture + 16384) / 32768.0;
        if (idx == 0) {
            this->simVars->setMixture1Pos(mixtureLeverPerc);
        }
        else {
            this->simVars->setMixture2Pos(mixtureLeverPerc);
        }
    }

public:
    void init()
    {
        printf("FdController init");

        this->simVars = new SimVars();

        float p = 150.0;
        float i = 580.0;
        float d = 0.0;
        this->mixtureController[0] = new PidController(p, i, d, -5, 5);
        this->mixtureController[1] = new PidController(p, i, d, -5, 5);
    }

    void update(int mixtureAxis[], double deltaTime)
    {
        this->mixtureAxis[0] = mixtureAxis[0];
        this->mixtureAxis[1] = mixtureAxis[1];
        this->updateMixture(deltaTime);
        this->updateVisibleMixture(0);
        this->updateVisibleMixture(1);
        this->simVars->setFadecActiveFlag();
    }
};

FdController FdCtrlInstance;

#endif // !FDCTRL
