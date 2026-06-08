#include "Core/Common.h"

#include "Displays/PFD/PFD.h"

#include "Plugin/PluginState.h"
#include "Core/AircraftState.h"
#include "Displays/AvionicsState.h"
#include "Displays/PFD/Horizon.h"
#include "Utils/Strings.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include <string>

// ------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------

static const int kScreenWidth  = 2560;
static const int kScreenHeight = 2048;
static const int kMargin       = 40;
static const int kLineHeight   = 60;
static char kDeviceID[] = "A220_PFD_PILOT";
static char kDeviceName[] = "A220 PFD (Pilot)";

// ------------------------------------------------------
// DRAWING HELPERS
// ------------------------------------------------------

static void DrawLine(int x, int& y, const std::string& text, float* color)
{
    XPLMDrawString(color, x, y, text.c_str(), nullptr, xplmFont_Basic);
    y -= kLineHeight;
}

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
//static float PFDBrightnessCallback(
//    float inRheoValue,
//    float inAmbientBrightness,
//    float inBusVoltsRatio,
//    void* inRefcon)
//{
//    // TEST: ignore power and rheostat — always output bright enough
//    // to punch through daytime HDR exposure.
//    const float kMaxBrightness = 5.0f;   // tune for daylight readability
//    return kMaxBrightness;
//}

static void DrawPFDScreen(void* inRefcon)
{
    PluginState* state = static_cast<PluginState*>(inRefcon);
    if (!state) return;

    const AircraftState& aircraft = state->aircraft;

    glDisable(GL_CULL_FACE);
    
    // Black background
    DrawBackground(0.01, 0.01f, 0.03f);

    DrawHorizon(aircraft, kScreenWidth, kScreenHeight);

    // Text on top
    XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);

    float white[] = { 1.0f, 1.0f, 1.0f };
    float cyan[]  = { 0.0f, 1.0f, 1.0f };

    int x = kMargin;
    int y = kScreenHeight - kMargin - kLineHeight;

    DrawLine(x, y, "A220 PFD", cyan);
    y -= kLineHeight / 2;

    DrawLine(x, y, Strings::Pad("IAS:",   Strings::Format(aircraft.flight.indicatedAirspeedKt, 1) + " kt"),  white);
    DrawLine(x, y, Strings::Pad("ALT:",   Strings::Format(aircraft.flight.altitudeFt,          0) + " ft"),  white);
    DrawLine(x, y, Strings::Pad("PITCH:", Strings::Format(aircraft.flight.pitchDeg,            1) + " deg"), white);
    DrawLine(x, y, Strings::Pad("ROLL:",  Strings::Format(aircraft.flight.rollDeg,             1) + " deg"), white);
    DrawLine(x, y, Strings::Pad("HDG:",   Strings::Format(aircraft.flight.yawDeg,              1) + " deg"), white);
}

// ------------------------------------------------------
// REGISTRATION
// ------------------------------------------------------
//static float PFDBrightnessCallback(
//    float inRheoValue,
//    float inAmbientBrightness,
//    float inBusVoltsRatio,
//    void* inRefcon)
//{
//    // No power = dark screen
//    if (inBusVoltsRatio <= 0.0f)
//        return 0.0f;
//
//    // Photo-cell behavior: use ambient brightness for readability,
//    // scaled by the brightness knob, gated by available bus voltage.
//    return inAmbientBrightness * inRheoValue * inBusVoltsRatio;
//}
//static float PFDBrightnessCallback(
//    float inRheoValue,
//    float inAmbientBrightness,
//    float inBusVoltsRatio,
//    void* inRefcon)
//{
//    // Track ambient light: bright in daylight to punch through exposure,
//    // dim at night so the screen doesn't flood the cockpit.
//    return inAmbientBrightness;
//}

void RegisterPFD(PluginState* state)
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

void UnregisterPFD(PluginState* state)
{
    if (!state) return;

    XPLMAvionicsID& id = state->avionics.pfdPilot;
    if (!id) return;

    XPLMDestroyAvionics(id);
    id = nullptr;
    XPLMDebugString("A220: PFD unregistered\n");
}

