#pragma once

struct PluginState;

namespace PFD
{
	void Register(PluginState* state);
	void Unregister(PluginState* state);
}