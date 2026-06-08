#pragma once

#include "Core/AircraftState.h"
#include "Core/State/DisplayState.h"
#include "Displays/AvionicsState.h"
#include "UI/DebugOverlayState.h"
#include "UI/MenuState.h"

struct PluginState
{
	AircraftState     aircraft;    // what the aircraft is doing
	AvionicsState     avionics;    // how displays are registered with X-Plane
	DisplayState      displays;    // what the screens are showing
	DebugOverlayState debug;       // debug window handle + state
	MenuState         menu;        // plugin menu state
};