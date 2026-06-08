#include "Core/Common.h"

#include "Sim/Interface/DataRefs/FlightDataRefs.h"

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

// ------------------------------------------------------
// PRIVATE HANDLES — internal to this translation unit
// ------------------------------------------------------

static XPLMDataRef sYaw = nullptr;
static XPLMDataRef sPitch = nullptr;
static XPLMDataRef sRoll = nullptr;
static XPLMDataRef sIAS = nullptr;
static XPLMDataRef sAltitude = nullptr;
static XPLMDataRef sBeacon = nullptr;

// ------------------------------------------------------
// INIT
// ------------------------------------------------------

bool DataRefs::Flight::Initialize()
{
    sYaw = XPLMFindDataRef("sim/flightmodel/position/true_psi");
    sPitch = XPLMFindDataRef("sim/flightmodel/position/theta");
    sRoll = XPLMFindDataRef("sim/flightmodel/position/phi");
    sIAS = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
    sAltitude = XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_pilot");

    if (!sRoll)     XPLMDebugString("A220: ROLL DataRef missing\n");
    if (!sPitch)    XPLMDebugString("A220: PITCH DataRef missing\n");
    if (!sYaw)      XPLMDebugString("A220: YAW DataRef missing\n");
    if (!sIAS)      XPLMDebugString("A220: IAS DataRef missing\n");
    if (!sAltitude) XPLMDebugString("A220: ALT DataRef missing\n");

    return sRoll && sPitch && sYaw && sIAS && sAltitude;
}

// ------------------------------------------------------
// GETTERS
// ------------------------------------------------------

float DataRefs::Flight::GetRoll() { return sRoll ? XPLMGetDataf(sRoll) : 0.0f; }
float DataRefs::Flight::GetPitch() { return sPitch ? XPLMGetDataf(sPitch) : 0.0f; }
float DataRefs::Flight::GetYaw() { return sYaw ? XPLMGetDataf(sYaw) : 0.0f; }
float DataRefs::Flight::GetIAS() { return sIAS ? XPLMGetDataf(sIAS) : 0.0f; }
float DataRefs::Flight::GetAltitude() { return sAltitude ? XPLMGetDataf(sAltitude) : 0.0f; }

// ------------------------------------------------------
// SETTERS
// ------------------------------------------------------
