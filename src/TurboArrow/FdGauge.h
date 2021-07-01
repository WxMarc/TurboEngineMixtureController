#pragma once


#ifndef FDGAUGE
#define FDGAUGE

#ifndef __INTELLISENSE__
#	define MODULE_EXPORT __attribute__( ( visibility( "default" ) ) )
#	define MODULE_WASM_MODNAME(mod) __attribute__((import_module(mod)))
#else
#	define MODULE_EXPORT
#	define MODULE_WASM_MODNAME(mod)
#	define __attribute__(x)
#	define __restrict__
#endif

#include "common.h"
#include "FdController.h"

const int MIN_MIX = -16384;
const int MAX_MIX = 16384;
const int MIX_STEP = 256;

int globalMixtureAxis = MAX_MIX;

class FdGauge
{
private:

    bool isConnected = false;

    /// <summary>
    /// Registers all the mixture SimConnect client events.
    /// </summary>
    void RegisterMixtureClientEvents()
    {
        printf("Registering mixture events...\r\n");

        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::AxisMixtureSet, "AXIS_MIXTURE_SET");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::AxisMixture1Set, "AXIS_MIXTURE1_SET");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureSetBest, "MIXTURE_SET_BEST");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureSet, "MIXTURE_SET");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureRich, "MIXTURE_RICH");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureIncr, "MIXTURE_INCR");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureIncrSmall, "MIXTURE_INCR_SMALL");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureDecrSmall, "MIXTURE_DECR_SMALL");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureDecr, "MIXTURE_DECR");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::MixtureLean, "MIXTURE_LEAN");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1Set, "MIXTURE1_SET");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1Rich, "MIXTURE1_RICH");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1Incr, "MIXTURE1_INCR");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1IncrSmall, "MIXTURE1_INCR_SMALL");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1DecrSmall, "MIXTURE1_DECR_SMALL");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1Decr, "MIXTURE1_DECR");
        SimConnect_MapClientEventToSimEvent(hSimConnect, MixtureEventIDs::Mixture1Lean, "MIXTURE1_LEAN");
    }

    /// <summary>
    /// Registers the SimConnect mixture event group for capture.
    /// </summary>
    void RegisterMixtureEventGroup()
    {
        printf("Registering mixture event group...\r\n");
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::AxisMixtureSet, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::AxisMixture1Set, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureSetBest, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureSet, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureRich, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureIncr, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureIncrSmall, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureDecrSmall, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureDecr, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::MixtureLean, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1Set, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1Rich, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1Incr, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1IncrSmall, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1DecrSmall, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1Decr, TRUE);
        SimConnect_AddClientEventToNotificationGroup(hSimConnect, EventGroups::Mixture, MixtureEventIDs::Mixture1Lean, TRUE);

        SimConnect_SetNotificationGroupPriority(hSimConnect, EventGroups::Mixture, SIMCONNECT_GROUP_PRIORITY_HIGHEST_MASKABLE);
    }

    /// <summary>
    /// Initializes the connection to SimConnect.
    /// </summary>
    /// <returns>True if successful, false otherwise.</returns>
    bool InitializeSimConnect()
    {
        printf("Connecting to SimConnect...\r\n");
        if (SUCCEEDED(SimConnect_Open(&hSimConnect, "FdGauge", nullptr, 0, 0, 0)))
        {
            printf("SimConnect connected.\r\n");

            this->RegisterMixtureClientEvents();
            this->RegisterMixtureEventGroup();

            SimConnect_AddToDataDefinition(hSimConnect, DataTypes::EngineControls, "GENERAL ENG MIXTURE LEVER POSITION:1", "Percent");

            printf("SimConnect registrations complete.\r\n");
            return true;
        }

        printf("SimConnect failed.\r\n");

        return false;
    }

    /// <summary>
    /// A callback used for handling SimConnect updates.
    /// </summary>
    /// <param name="pData">The update data sent by SimConnect.</param>
    /// <param name="cbData">The size of the SimConnect data structure.</param>
    /// <param name="pContext">A pointer specified by the client.</param>
    static void CALLBACK HandleAxisEvent(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
    {
        if (pData->dwID == SIMCONNECT_RECV_ID::SIMCONNECT_RECV_ID_EVENT)
        {
            SIMCONNECT_RECV_EVENT* evt = static_cast<SIMCONNECT_RECV_EVENT*>(pData);
            if (evt->uGroupID == EventGroups::Mixture)
            {
                FdGauge* fd = static_cast<FdGauge*>(pContext);
                if (fd == 0)
                {
                    printf("FD pointer was null processing SimConnect event.\r\n");
                }
                else
                {
                    HandleMixtureAxis(evt);
                }
            }
        }

        if (pData->dwID == SIMCONNECT_RECV_ID::SIMCONNECT_RECV_ID_EXCEPTION)
        {
            SIMCONNECT_RECV_EXCEPTION* ex = static_cast<SIMCONNECT_RECV_EXCEPTION*>(pData);
            printf("SimConnect Exception: %d \r\n", ex->dwException);
        }
    }

    /// <summary>
    /// Handles throttle axis updates received from SimConnect.
    /// </summary>
    /// <param name="evt">A pointer to the SimConnect event structure.</param>
    static void HandleMixtureAxis(SIMCONNECT_RECV_EVENT* evt)
    {
        switch (evt->uEventID)
        {
        case MixtureEventIDs::AxisMixtureSet:
        case MixtureEventIDs::AxisMixture1Set:
            globalMixtureAxis = static_cast<int>(evt->dwData);
            break;
        case MixtureEventIDs::MixtureSetBest:
        case MixtureEventIDs::MixtureRich:
        case MixtureEventIDs::Mixture1Rich:
            globalMixtureAxis = MAX_MIX;
            break;
        case MixtureEventIDs::MixtureSet:
            globalMixtureAxis = (static_cast<int>(evt->dwData) * 2) - MAX_MIX;
            break;
        case MixtureEventIDs::Mixture1Set:
            globalMixtureAxis = (static_cast<int>(evt->dwData) * 2) - MAX_MIX;
            break;
        case MixtureEventIDs::MixtureLean:
        case MixtureEventIDs::Mixture1Lean:
            globalMixtureAxis = MIN_MIX;
            break;
        case MixtureEventIDs::MixtureIncr:
        case MixtureEventIDs::MixtureIncrSmall:
        case MixtureEventIDs::Mixture1Incr:
        case MixtureEventIDs::Mixture1IncrSmall:
            globalMixtureAxis += MIX_STEP; // TODO: CLAMP ALL INCR/DECR EVENTS
            break;
        case MixtureEventIDs::MixtureDecr:
        case MixtureEventIDs::MixtureDecrSmall:
        case MixtureEventIDs::Mixture1Decr:
        case MixtureEventIDs::Mixture1DecrSmall:
            globalMixtureAxis -= MIX_STEP; // TODO: CLAMP ALL INCR/DECR EVENTS
            break;
        }

        globalMixtureAxis = clamp(globalMixtureAxis, MIN_MIX, MAX_MIX);
    }

public:

    /// <summary>
    /// Initializes the FD.
    /// </summary>
    /// <returns>True if successful, false otherwise.</returns>
    bool InitializeFD()
    {
        if (!this->InitializeSimConnect()) {
            printf("Init SimConnect failed");
            return false;
        }

        FdCtrlInstance.init();
        isConnected = true;
        SimConnect_CallDispatch(hSimConnect, HandleAxisEvent, this);

        return true;
    }

    /// <summary>
    /// A callback used to update the FD at each tick.
    /// </summary>
    /// <param name="deltaTime">The time since the previous update.</param>
    /// <returns>True if successful, false otherwise.</returns>
    bool OnUpdate(double deltaTime)
    {
        if (isConnected == true) {
            FdCtrlInstance.update(globalMixtureAxis, deltaTime);
        }

        return true;
    }

    /// <summary>
    /// Kill.
    /// </summary>
    /// <returns>True if succesful, false otherwise.</returns>
    bool KillFD()
    {
        isConnected = false;
        unregister_all_named_vars();
        return SUCCEEDED(SimConnect_Close(hSimConnect));
    }
};

#endif // !FDGAUGE
