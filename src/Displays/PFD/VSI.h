#pragma once

#include "Core/State/FlightState.h" // FlightState
#include "Displays/Common/Geometry.h" // Rect

namespace PFD
{
	void DrawVSI(const Rect& frame, const FlightState& flight); // will need to be swapped to GuidanceMode struct
}