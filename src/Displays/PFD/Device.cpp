#include "Core/Common.h"

#include "Displays/PFD/Device.h"

#include "Plugin/PluginState.h"
#include "Core/AircraftState.h"
#include "Displays/AvionicsState.h"
#include "Displays/PFD/Horizon.h"
#include "Displays/PFD/AirspeedTape.h"
#include "Displays/PFD/Layout.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

namespace PFD
{
	// ------------------------------------------------------
	// CONSTANTS
	// ------------------------------------------------------

	static char kDeviceID[] = "A220_PFD_PILOT";
	static char kDeviceName[] = "A220 PFD (Pilot)";

	// ------------------------------------------------------
	// DRAWING HELPERS
	// ------------------------------------------------------

	static void DrawBackground(float r, float g, float b)
	{
		XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
		glColor4f(r, g, b, 1.0f);
		glBegin(GL_QUADS);
		glVertex2f(kScreenWidth, 0);
		glVertex2f(0, 0);
		glVertex2f(0, kScreenHeight);
		glVertex2f(kScreenWidth, kScreenHeight);
		glEnd();
	}

	// ------------------------------------------------------
	// DRAW CALLBACK
	// ------------------------------------------------------

	static void DrawPFDScreen(void* inRefcon)
	{
		PluginState* state = static_cast<PluginState*>(inRefcon);
		if (!state) return;

		const AircraftState& aircraft = state->aircraft;

		glDisable(GL_CULL_FACE);
		
		// Black background
		DrawBackground(0.01f, 0.01f, 0.03f);

		const AttitudeRegion region = MakeAttitudeRegion(kScreenWidth, kScreenHeight);

		DrawHorizon(aircraft, region);
		
		DrawAircraftRefSymbol(region);

		DrawTapes(region);
	}

	// ------------------------------------------------------
	// REGISTRATION
	// ------------------------------------------------------

	void Register(PluginState* state)
	{
		if (!state) return;

		XPLMCreateAvionics_t params{};
		params.structSize = sizeof(params);
		params.screenWidth = kScreenWidth;
		params.screenHeight = kScreenHeight;
		params.bezelWidth = kScreenWidth;
		params.bezelHeight = kScreenHeight;
		params.screenOffsetX = 0;
		params.screenOffsetY = 0;
		params.drawOnDemand = 0;
		params.drawCallback = DrawPFDScreen;
		params.deviceID = kDeviceID;
		params.deviceName = kDeviceName;
		params.refcon = state;
		// Null out all optional callbacks
		params.bezelDrawCallback = nullptr;
		params.bezelClickCallback = nullptr;
		params.bezelRightClickCallback = nullptr;
		params.bezelScrollCallback = nullptr;
		params.bezelCursorCallback = nullptr;
		params.screenTouchCallback = nullptr;
		params.screenRightTouchCallback = nullptr;
		params.screenScrollCallback = nullptr;
		params.screenCursorCallback = nullptr;
		params.keyboardCallback = nullptr;
		//params.brightnessCallback = PFDBrightnessCallback;
		params.brightnessCallback = nullptr;

		XPLMAvionicsID id = XPLMCreateAvionicsEx(&params);

		if (!id)
		{
			XPLMDebugString("A220: Failed to create PFD avionics device\n");
			return;
		}

		state->avionics.pfdPilot = id;
		XPLMDebugString("A220: PFD registered\n");
	}

	void Unregister(PluginState* state)
	{
		if (!state) return;

		XPLMAvionicsID& id = state->avionics.pfdPilot;
		if (!id) return;

		XPLMDestroyAvionics(id);
		id = nullptr;
		XPLMDebugString("A220: PFD unregistered\n");
	}
}




//static void DrawLine(int x, int& y, const std::string& text, float* color)
//{
//    XPLMDrawString(color, x, y, text.c_str(), nullptr, xplmFont_Basic);
//    y -= kLineHeight;
//}
