#pragma once

#include <string>

#include "Plugin/PluginState.h"
#include "XPLMDisplay.h"

struct DebugLine {
	std::string lable;
	std::string value;
};

void RegisterDebugWindow(PluginState* state);
void UnregisterDebugWindow(PluginState* state);
void ToggleDebugWindow(PluginState* state);
