#include "Core/Common.h"

#include "Systems.h"

#include "Core/AircraftState.h"

void UpdateSystems(
	AircraftState& state, 
	float dt) 
{
	if (state.flight.indicatedAirspeedKt > 10.0f) {
		state.lighting.beaconLightOn = true;
	}
	else {
		state.lighting.beaconLightOn = false;
	}

	//UpdateAirConditioning()

	//UpdateAutoflight();

	//UpdateAutopilot()

	//UpdateCargo();

	//UpdateCommuncations();

	//UpdateDoors();

	//UpdateElectrical();

	//UpdateEngines()

	//UpdateFailures()

	//UpdateFireProtection();
	 
	//UpdateFlightControls();

	//UpdateFuel();

	//UpdateFMS();

	//UpdateHydraulics();

	//UpdateIceProtection();

	//UpdateLandingGear();

	//UpdateLighting();

	//UpdateNavigation();

	//UpdateOxygen();

	//UpdatePneumatics();

	//UpdatePressurization();

}
