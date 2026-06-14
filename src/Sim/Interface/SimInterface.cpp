#include "Core/Common.h"

#include "Sim/Interface/SimInterface.h"

#include "Core/AircraftState.h"
#include "Sim/Interface/DataRefs/DataRefs.h"

#include "XPLMUtilities.h"

#include <cstdio>

void ReadSim(AircraftState& state) {
	// Flight
	state.flight.rollDeg             = DataRefs::Flight::GetRoll();
	state.flight.pitchDeg            = DataRefs::Flight::GetPitch();
	state.flight.yawDeg              = DataRefs::Flight::GetYaw();
	state.flight.altitudeFt          = DataRefs::Flight::GetAltitude();
	state.flight.indicatedAirspeedKt = DataRefs::Flight::GetIAS();
    state.flight.verticalSpeedFpm    = DataRefs::Flight::GetVFpm();

    // Engines
    for (int i = 0; i < 2; ++i)
    {
        state.engines[i].n1          = DataRefs::Engine::GetN1(i);
        state.engines[i].n2          = DataRefs::Engine::GetN2(i);
        state.engines[i].egt         = DataRefs::Engine::GetEGT(i);
        state.engines[i].fuelFlow    = DataRefs::Engine::GetFuelFlow(i);
        state.engines[i].oilPressure = DataRefs::Engine::GetOilPressure(i);
        state.engines[i].oilTemp     = DataRefs::Engine::GetOilTemp(i);
        state.engines[i].running     = DataRefs::Engine::GetRunning(i);
        state.engines[i].starting    = DataRefs::Engine::GetStarting(i);
        state.engines[i].fireHandle  = DataRefs::Engine::GetFireHandle(i);
    }

	// Lighting
	state.lighting.beaconLightOn     = DataRefs::Lighting::GetBeaconLight();
}

void WriteSim(const AircraftState& state) {
	DataRefs::Lighting::SetBeaconLight(state.lighting.beaconLightOn);
}
