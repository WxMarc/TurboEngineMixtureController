#pragma once

#ifndef SCDEFS
#define SCDEFS

#include "common.h"

/// <summary>
/// SimConnect client event IDs for the mixture group.
/// </summary>
enum MixtureEventIDs
{
    AxisMixtureSet,
    AxisMixture1Set,
    AxisMixture2Set,
    MixtureSetBest,
    MixtureSet,
    MixtureRich,
    MixtureIncr,
    MixtureIncrSmall,
    MixtureDecrSmall,
    MixtureDecr,
    MixtureLean,
    Mixture1Set,
    Mixture1Rich,
    Mixture1Incr,
    Mixture1IncrSmall,
    Mixture1DecrSmall,
    Mixture1Decr,
    Mixture1Lean,
    Mixture2Set,
    Mixture2Rich,
    Mixture2Incr,
    Mixture2IncrSmall,
    Mixture2DecrSmall,
    Mixture2Decr,
    Mixture2Lean
};

/// <summary>
/// SimConnect event groups.
/// </summary>
enum EventGroups
{
    /// <summary>
    /// The client event group ID to use when any events from the mixture axis group
    /// are received.
    /// </summary>
    Mixture = 0,
};

/// <summary>
/// SimConnect data types for sending the sim updates.
/// </summary>
enum DataTypes
{
    /// <summary>
    /// The data type ID to use when sending engine controls data.
    /// </summary>
    EngineControls = 0
};
/// <summary>
/// Engine controls.
/// </summary>
struct EngineControlData
{
    /// <summary>
    /// The mixture of the engine, expressed in a 100s base percent.
    /// </summary>
    double mixtureLeft;
    double mixtureRight;
};

/// <summary>
/// A collection of SimVar unit enums.
/// </summary>
class Units
{
public:
    /// <summary>
    /// The Percent SimVar unit.
    /// </summary>
    ENUM Percent = get_units_enum("Percent");

    /// <summary>
    /// The Ratio SimVar unit.
    /// </summary>
    ENUM Ratio = get_units_enum("Ratio");

    ENUM Celsius = get_units_enum("Celsius");

    ENUM Number = get_units_enum("Number");

    ENUM inHg = get_units_enum("inches of mercury");
};


/// <summary>
/// A collection of SimVar enums.
/// </summary>
class SimVars
{
public:
    Units* m_Units;

    /// <summary>
    /// The GENERAL ENGINE MIXTURE LEVER POSITION SimVar.
    /// </summary>
    ENUM Mixture = get_aircraft_var_enum("GENERAL ENG MIXTURE LEVER POSITION");

    ENUM FuelAirRatio = get_aircraft_var_enum("RECIP MIXTURE RATIO");

    ENUM AmbientTemperature = get_aircraft_var_enum("AMBIENT TEMPERATURE");

    ENUM AmbientPressure = get_aircraft_var_enum("AMBIENT PRESSURE");

    ID FadecActive;

    /// <summary>
    /// The local variable for the visible mixture position
    /// </summary>
    ID MixturePos1;
    ID MixturePos2;

    SimVars()
    {
        this->initializeVars();
    }

    void initializeVars() {
        FadecActive = register_named_variable("FADEC_ACTIVE");
        MixturePos1 = register_named_variable("Mixture1_Pos");
        MixturePos2 = register_named_variable("Mixture2_Pos");
        m_Units = new Units();
    }

    void setFadecActiveFlag() {
        set_named_variable_value(FadecActive, 1);
    }

    void setMixture1Pos(double value) {
        set_named_variable_value(MixturePos1, value);
    }

    void setMixture2Pos(double value) {
        set_named_variable_value(MixturePos2, value);
    }

    FLOAT64 getMixtureLeverPosition(int index) {
        return aircraft_varget(Mixture, m_Units->Percent, index);
    }

    FLOAT64 getFuelAirRatio(int index) {
        return aircraft_varget(FuelAirRatio, m_Units->Ratio, index);
    }

    FLOAT64 getAmbientTemperature() {
        return aircraft_varget(AmbientTemperature, m_Units->Celsius, 0);
    }

    FLOAT64 getAmbientPressure() {
        return aircraft_varget(AmbientPressure, m_Units->inHg, 0);
    }
};

#endif
