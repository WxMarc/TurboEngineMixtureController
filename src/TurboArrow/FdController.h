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
    PidController* mixtureController;

    /// <summary>
    /// The current mixture control axis, from -16384 to 16384.
    /// </summary>  
    int mixtureAxis = 16384;

    bool enabled = true;

    /// <summary>
    /// Calculates and updates mixture according to target and PIDs.
    /// </summary>
    /// <param name="deltaTime"></param>
    void updateMixture(double deltaTime) {
        EngineControlData controls;
        controls.mixtureone = this->getDesiredMixture(deltaTime);
        SimConnect_SetDataOnSimObject(hSimConnect, DataTypes::EngineControls, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(EngineControlData), &controls);
    }

    /// <summary>
    /// Gets the mixture lever position to achieve the desired fuel/air mixture ratio
    /// </summary>
    /// <returns>Desired mixture setting</returns>
    double getDesiredMixture(double deltaTime) {
        double mixtureLeverPerc = 100.0 * (this->mixtureAxis + 16384) / 32768.0;

        if (!enabled) {
            return mixtureLeverPerc;
        }

        double targetFuelAirMixture = Turbocharger::getTargetFuelAirRatio(this->simVars->getAmbientTemperature(), this->simVars->getAmbientPressure(), mixtureLeverPerc);
        double simFuelAirMixture = this->simVars->getFuelAirRatio(1);
        printf("Target Mixture: %1.3f\nSim Mixture: %1.3f\n", targetFuelAirMixture, simFuelAirMixture);

        double error = targetFuelAirMixture - simFuelAirMixture;
        double pidOut = 0.0;
        pidOut = this->mixtureController->GetOutput(error, deltaTime);
        printf("Output from PID Controller: %2.3f\n", error);
        return max(0, min(100, this->simVars->getMixtureLeverPosition(1) + pidOut));
    }

    void updateVisibleMixture() {
        int targetMixture = this->mixtureAxis;
        double mixtureLeverPerc = 100.0 * (targetMixture + 16384) / 32768.0;
        this->simVars->setMixturePos(mixtureLeverPerc);
    }

public:
    void init()
    {
        printf("FdController init");

        this->simVars = new SimVars();

        float p = 350.0;
        float i = 380.0;
        float d = 0.0;
        this->mixtureController = new PidController(p, i, d, -5, 5);
    }

    void update(int mixtureAxis, double deltaTime)
    {
        this->mixtureAxis = mixtureAxis;
        this->updateMixture(deltaTime);
        this->updateVisibleMixture();
        this->simVars->setFadecActiveFlag();
    }
};

FdController FdCtrlInstance;

#endif // !FDCTRL
