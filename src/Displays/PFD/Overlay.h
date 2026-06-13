#pragma once

#include "Displays/PFD/Layout.h"   // AttitudeRegion

struct AircraftState;

namespace PFD { 
	void DrawOverlay(const AircraftState& aircraft, const AttitudeRegion& region); 
}