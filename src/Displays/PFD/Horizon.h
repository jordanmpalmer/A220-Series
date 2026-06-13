#pragma once

#include "Displays/Common/Geometry.h"

struct AircraftState;

namespace PFD
{
	void DrawHorizon(const AircraftState& aicraft, const PFD::AttitudeRegion& region);
	void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region);
}
