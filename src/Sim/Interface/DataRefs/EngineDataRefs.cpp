#include "Core/Common.h"

#include "Sim/Interface/DataRefs/EngineDataRefs.h"

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

// ------------------------------------------------------
// PRIVATE HANDLES — internal to this translation unit
// ------------------------------------------------------

static XPLMDataRef sN1 = nullptr;
static XPLMDataRef sN2 = nullptr;
static XPLMDataRef sEGT = nullptr;
static XPLMDataRef sFuelFlow = nullptr;
static XPLMDataRef sOilPressure = nullptr;
static XPLMDataRef sOilTemp = nullptr;
static XPLMDataRef sRunning = nullptr;
static XPLMDataRef sStarting = nullptr;
static XPLMDataRef sFireHandle = nullptr;

// ------------------------------------------------------
// INIT
// ------------------------------------------------------

bool DataRefs::Engine::Initialize()
{
    sN1 = XPLMFindDataRef("sim/cockpit2/engine/indicators/N1_percent");
    sN2 = XPLMFindDataRef("sim/cockpit2/engine/indicators/N2_percent");
    sEGT = XPLMFindDataRef("sim/cockpit2/engine/indicators/EGT_deg_cel");
    sFuelFlow = XPLMFindDataRef("sim/cockpit2/engine/indicators/fuel_flow_kg_sec");
    sOilPressure = XPLMFindDataRef("sim/cockpit2/engine/indicators/oil_pressure_psi");
    sOilTemp = XPLMFindDataRef("sim/cockpit2/engine/indicators/oil_temperature_deg_C");
    sRunning = XPLMFindDataRef("sim/flightmodel2/engines/engine_is_burning_fuel");
    sStarting = XPLMFindDataRef("sim/cockpit2/engine/actuators/ignition_on");
    sFireHandle = XPLMFindDataRef("sim/cockpit2/engine/actuators/fire_extinguisher_on");

    if (!sN1)          XPLMDebugString("A220: N1 DataRef missing\n");
    if (!sN2)          XPLMDebugString("A220: N2 DataRef missing\n");
    if (!sEGT)         XPLMDebugString("A220: EGT DataRef missing\n");
    if (!sFuelFlow)    XPLMDebugString("A220: FuelFlow DataRef missing\n");
    if (!sOilPressure) XPLMDebugString("A220: OilPressure DataRef missing\n");
    if (!sOilTemp)     XPLMDebugString("A220: OilTemp DataRef missing\n");
    if (!sRunning)     XPLMDebugString("A220: Running DataRef missing\n");
    if (!sStarting)    XPLMDebugString("A220: Starting DataRef missing\n");
    if (!sFireHandle)  XPLMDebugString("A220: FireHandle DataRef missing\n");

    return sN1 && sN2 && sEGT;
}

// ------------------------------------------------------
// HELPERS
// ------------------------------------------------------

static inline bool ValidEngine(int e) { return e == 0 || e == 1; }

// ------------------------------------------------------
// GETTERS
// ------------------------------------------------------

float DataRefs::Engine::GetN1(int e)
{
    if (!ValidEngine(e) || !sN1) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sN1, values, 0, 2);
    return values[e];
}

float DataRefs::Engine::GetN2(int e)
{
    if (!ValidEngine(e) || !sN2) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sN2, values, 0, 2);
    return values[e];
}

float DataRefs::Engine::GetEGT(int e)
{
    if (!ValidEngine(e) || !sEGT) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sEGT, values, 0, 2);
    return values[e];
}

float DataRefs::Engine::GetFuelFlow(int e)
{
    if (!ValidEngine(e) || !sFuelFlow) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sFuelFlow, values, 0, 2);
    return values[e] * 3600.0f;  // kg/sec → kg/hr
}

float DataRefs::Engine::GetOilPressure(int e)
{
    if (!ValidEngine(e) || !sOilPressure) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sOilPressure, values, 0, 2);
    return values[e];
}

float DataRefs::Engine::GetOilTemp(int e)
{
    if (!ValidEngine(e) || !sOilTemp) return 0.0f;
    float values[2] = { 0.0f, 0.0f };
    XPLMGetDatavf(sOilTemp, values, 0, 2);
    return values[e];
}

bool DataRefs::Engine::GetRunning(int e)
{
    if (!ValidEngine(e) || !sRunning) return false;
    int values[2] = { 0, 0 };
    XPLMGetDatavi(sRunning, values, 0, 2);
    return values[e] != 0;
}

bool DataRefs::Engine::GetStarting(int e)
{
    if (!ValidEngine(e) || !sStarting) return false;
    int values[2] = { 0, 0 };
    XPLMGetDatavi(sStarting, values, 0, 2);
    return values[e] != 0;
}

bool DataRefs::Engine::GetFireHandle(int e)
{
    if (!ValidEngine(e) || !sFireHandle) return false;
    int values[2] = { 0, 0 };
    XPLMGetDatavi(sFireHandle, values, 0, 2);
    return values[e] != 0;
}