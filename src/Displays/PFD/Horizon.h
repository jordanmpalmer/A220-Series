#pragma once

#include "Displays/PFD/Layout.h"

struct AircraftState;

namespace PFD
{
	void DrawHorizon(const AircraftState& aicraft, const PFD::AttitudeRegion& region);
	void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region);
}
