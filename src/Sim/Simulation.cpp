#include "Core/Common.h"

#include "Simulation.h"

#include "Core/AircraftState.h"
#include "Plugin/PluginState.h"
#include "Sim/Interface/SimInterface.h"
#include "Systems/Systems.h"
#include "Model/UpdateModel.h"

void RunSimulationFrame(PluginState& state, float dt)
{
	AircraftState& aircraft = state.aircraft;
	// FlightLoop();

	// 1. READ (FROM XP) pull raw data from X-Plane into AircraftState 
	ReadSim(aircraft);

	// UpdateEnvironment(); // ISA atmosphere, wind, temperature, pressure 

	// 2. MODEL PHASE (Pyhsics and Performance) physics, performance, lift/drag/thrust 
	UpdateModel(aircraft, dt);

	// 3. SYSTEM PHASE (Systems Logic) electrical, fuel, hydraulic, pneumatic etc. 
	UpdateSystems(aircraft, dt);

	// UpdateAutoflight(); // autopilot, autothrottle, FMS 

	// UpdateFailures(); // apply any active failures to state

	// 4. WRITE (TO XP) push AircraftState back to X-Plane 
	WriteSim(aircraft);
}
