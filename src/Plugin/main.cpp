#include "Core/Common.h"

#include "XPLMUtilities.h"
#include "XPLMProcessing.h"
#include "XPLMMenus.h"
#include "XPLMPlugin.h"

#include "Plugin/PluginState.h"
#include "Plugin/FlightLoop.cpp"
#include "Displays/PFD/Device.h"
#include "Sim/Interface/SimInterface.h"
#include "Sim/Interface/DataRefs/DataRefs.h"
#include "UI/Debug.h"
#include "UI/Menu.h"

static PluginState gState;

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
    std::strcpy(outName, "A220-Series");
    std::strcpy(outSig, "jordanpalmer.a220series");
    std::strcpy(outDesc, "A220-Series development plugin");

    XPLMDebugString("A220-Series: Plugin Initializing\n");

	if (!DataRefs::Initialize())
	{
		XPLMDebugString("A220-Series: Failed to initialize datarefs\n");
		return 0;
	}

	RegisterMenus(&gState);
	RegisterDebugWindow(&gState);
	PFD::Register(&gState);
	XPLMRegisterFlightLoopCallback(AircraftFlightLoopCallback, -1.0f, &gState);

    XPLMDebugString("A220-Series: Plugin Started\n");

    return 1;
}

PLUGIN_API void XPluginStop()
{
	UnregisterMenus(&gState);
	UnregisterDebugWindow(&gState);
	PFD::Unregister(&gState);
	XPLMUnregisterFlightLoopCallback(AircraftFlightLoopCallback, nullptr);

    XPLMDebugString("A220-Series: XPluginStop called\n");
}

PLUGIN_API int XPluginEnable()
{
    XPLMDebugString("A220-Series: XPluginEnable called\n");

    return 1;
}

PLUGIN_API void XPluginDisable()
{
    XPLMDebugString("A220-Series: XPluginDisable called\n");
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}

//// Draw callback — separate X-Plane callback entirely
//static void DrawCallback(XPLMWindowID window, void* refcon)
//{
//	RenderDisplays(gAircraft);  // read-only, just visualizes state
//}

