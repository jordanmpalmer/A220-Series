#pragma once

#include "Core/State/DisplayState.h"
#include "Core/State/ElectricalState.h"
#include "Core/State/EngineState.h"
#include "Core/State/EnvironmentState.h"
#include "Core/State/FailureState.h"
#include "Core/State/FlightControlState.h"
#include "Core/State/FlightState.h"
#include "Core/State/FuelState.h"
#include "Core/State/HydraulicState.h"
#include "Core/State/LandingGearState.h"
#include "Core/State/LightingState.h"
#include "Core/State/PneumaticState.h"
#include "Core/State/PressurizationState.h"

struct AircraftState {

	ElectricalState electrical;

	EngineState engines[2]; // [0] = ENG1, [1] = ENG2

	EnvironmentState  environment; // what the world is doing

	FailureState failures;

	FlightControlState flightControls;

	FlightState flight;

	FuelState fuel;

	HydraulicState hydraulics;

	LandingGearState landingGear;

	LightingState lighting;

	PneumaticState pneumatic;

	PressurizationState pressurization;

	// Debug
	float dt = 0.0f;
};
