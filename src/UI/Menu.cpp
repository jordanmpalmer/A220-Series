#include "Core/Common.h"

#include "Menu.h"

#include "Debug.h"

#include "XPLMDisplay.h"
#include "XPLMMenus.h"
#include "XPLMPlugin.h"
#include "XPLMUtilities.h"

static void AircraftPluginMenuHandler(void* mRef, void* iRef)
{
	PluginState* state = static_cast<PluginState*>(mRef);
	if (!state) return;

	const char* cmd = static_cast<const char*>(iRef);
	if (!cmd) return;

	if (!strcmp(cmd, "ToggleDebug"))
	{
		ToggleDebugWindow(state);
	}
	else if (!strcmp(cmd, "ReloadAircraftPlugin"))
	{
		XPLMDebugString("A220-Series: Reloading plugin\n");
		XPLMReloadPlugins();
	}
	else if (strcmp(cmd, "TogglePFD") == 0)
	{
		if (state->avionics.pfdPilot)
		{
			XPLMPopOutAvionics(state->avionics.pfdPilot);
		}
	}
}

void RegisterMenus(PluginState* state)
{
	if (!state) return;

	XPLMMenuID pluginsMenu = XPLMFindPluginsMenu();
	int menuName = XPLMAppendMenuItem(pluginsMenu, "A220-Series", NULL, 1);
	state->menu.menuID = XPLMCreateMenu("A220-Series", pluginsMenu, menuName, AircraftPluginMenuHandler, state);

	XPLMAppendMenuItem(state->menu.menuID, "Reload",              (void*)"ReloadAircraftPlugin", 1);
	XPLMAppendMenuItem(state->menu.menuID, "Toggle Debug Window", (void*)"ToggleDebug",          1);
	XPLMAppendMenuItem(state->menu.menuID, "Toggle PFD",          (void*)"TogglePFD",            1);
}

void UnregisterMenus(PluginState* state)
{
	if (!state) return;

	XPLMMenuID& menu = state->menu.menuID;
	if (!menu) return;

	XPLMDestroyMenu(menu);
	menu = nullptr;
}