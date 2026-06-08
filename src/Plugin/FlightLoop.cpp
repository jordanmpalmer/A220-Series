#include "Core/Common.h"

#include "Plugin/PluginState.h"
#include "Sim/Simulation.h"
#include "Utils/Time.h"

static float AircraftFlightLoopCallback(
	float,
	float elapsed,
	int,
	void* inRefcon)
{
	PluginState* state = static_cast<PluginState*>(inRefcon);

	const float dt = Time::Clamp(elapsed);

	RunSimulationFrame(*state, dt);

	return -1.0f;
}
