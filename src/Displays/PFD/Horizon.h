#pragma once

struct AircraftState;

namespace PFD
{
	struct AttitudeRegion;
};

void DrawHorizon(const AircraftState& aicraft, const PFD::AttitudeRegion& region);
void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region);
