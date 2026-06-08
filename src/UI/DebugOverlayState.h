#pragma once

#include "XPLMDisplay.h"

enum class DebugTab
{
	Flight = 0,
	Engines,
	Electrical,
	Fuel,
	Count
};

struct DebugOverlayState
{
	XPLMWindowID windowID  = nullptr;
	DebugTab     activeTab = DebugTab::Flight;
};