#pragma once

struct AircraftState;

namespace PFD
{
	struct AttitudeRegion;
};

void DrawHorizon(const AircraftState& aicraft, const PFD::AttitudeRegion& region);
void DrawAircraftRefSymbol(const PFD::AttitudeRegion& region);
void DrawAltitudeTape(const PFD::AttitudeRegion& region);
void DrawSpeedTape(const PFD::AttitudeRegion& region);
void DrawTopBar(const PFD::AttitudeRegion& region);
