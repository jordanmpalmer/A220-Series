#include "Core/Common.h"

#include "Sim/Interface/DataRefs/LightingDataRefs.h"

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

// ------------------------------------------------------
// PRIVATE HANDLES — internal to this translation unit
// ------------------------------------------------------

static XPLMDataRef sBeacon = nullptr;

// ------------------------------------------------------
// INIT
// ------------------------------------------------------

bool DataRefs::Lighting::Initialize()
{
    sBeacon = XPLMFindDataRef("sim/cockpit2/switches/beacon_on");

    if (!sBeacon) XPLMDebugString("A220: BEACON DataRef missing\n");

    //if (!sBeacon)
    //    XPLMDebugString("A220: BEACON DataRef missing - beacon will not function\n");

    return sBeacon;
}

// ------------------------------------------------------
// GETTERS
// ------------------------------------------------------

bool DataRefs::Lighting::GetBeaconLight() { return sBeacon ? XPLMGetDatai(sBeacon) != 0 : false; }

// ------------------------------------------------------
// SETTERS
// ------------------------------------------------------

void DataRefs::Lighting::SetBeaconLight(bool on) { if (sBeacon) XPLMSetDatai(sBeacon, on ? 1 : 0); }
