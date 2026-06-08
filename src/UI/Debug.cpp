#include "Core/Common.h"

#include "Debug.h"

#include "Utils/Strings.h"
#include "Plugin/PluginState.h"
#include "Core/AircraftState.h"
#include "Core/State/EngineState.h"
#include "UI/DebugOverlayState.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMPlugin.h"
#include "XPLMUtilities.h"

#include "string"

static const int kMargin     = 10;
static const int kLineHeight = 16;
static const int kTabHeight  = 24;

const char* kReloadButtonText  = "Reload Plugin";
int         kReloadButtonWidth = static_cast<int>(XPLMMeasureString(xplmFont_Basic, kReloadButtonText, static_cast<int>(strlen(kReloadButtonText))));

// ------------------------------------------------------
// DRAWING HELPERS
// ------------------------------------------------------

static void DrawLine(int x, int& y, const std::string& text, float r = 1.f, float g = 1.f, float b = 1.f)
{
    float color[] = { r, g, b };
    XPLMDrawString(color, x, y, text.c_str(), nullptr, xplmFont_Basic);
    y -= kLineHeight;
}

static void DrawLineInPlace(int x, int y, const std::string& text, float r = 1.f, float g = 1.f, float b = 1.f)
{
    float color[] = { r, g, b };
    XPLMDrawString(color, x, y, text.c_str(), nullptr, xplmFont_Basic);
}

static void DrawHeader(int x, int& y, const std::string& text)
{
    float color[] = { 0.6f, 0.8f, 1.0f };
    XPLMDrawString(color, x, y, text.c_str(), nullptr, xplmFont_Proportional);
    y -= kLineHeight;
}

static void DrawSeparator(int x, int& y, int l, int r)
{
    float charWidth = XPLMMeasureString(xplmFont_Basic, "-", 1);
    int count       = (int)((r - l - kMargin * 2) / charWidth);
    std::string sep(count, '-');
    DrawLine(x, y, sep, 0.4f, 0.4f, 0.4f);
}


// ------------------------------------------------------
// TAB BAR
// ------------------------------------------------------

static const char* TabName(DebugTab tab)
{
    switch (tab)
    {
        case DebugTab::Flight:     return "Flight";
        case DebugTab::Engines:    return "Engines";
        case DebugTab::Electrical: return "Electrical";
        case DebugTab::Fuel:       return "Fuel";
        default:                   return "";
    }
}

static void DrawTabBar(XPLMWindowID window, int l, int t, int r, DebugTab activeTab)
{
    int tabCount = (int)DebugTab::Count;
    int tabWidth = (r - l) / tabCount;

    for (int i = 0; i < tabCount; i++)
    {
        DebugTab tab = (DebugTab)i;
        int tx = l + (i * tabWidth);

        bool  active = (tab == activeTab);
        float col    = active ? 1.0f : 0.5f;
        float color[] = { col, col, col };

        XPLMDrawString(color, tx + 4, t - 14, TabName(tab), nullptr, xplmFont_Basic);
    }

    // Separator under tab bar — now spans full width
    float charWidth = XPLMMeasureString(xplmFont_Basic, "-", 1);
    int   count     = (int)((r - l - kMargin * 2) / charWidth);
    std::string sep(count, '-');
    float sepColor[] = { 0.4f, 0.4f, 0.4f };
    XPLMDrawString(sepColor, l + kMargin, t - kTabHeight, sep.c_str(), nullptr, xplmFont_Basic);
}

// ------------------------------------------------------
// TAB CONTENT
// ------------------------------------------------------

static void DrawFlightTab(int x, int& y, int l, int r, const AircraftState& state)
{
    DrawHeader(x, y, "FLIGHT");
    DrawSeparator(x, y, l, r);
    DrawLine(x, y, Strings::Pad("IAS:", Strings::Format(state.flight.indicatedAirspeedKt, 1) + " kt"));
    DrawLine(x, y, Strings::Pad("ALT:", Strings::Format(state.flight.altitudeFt,          0) + " ft"));
    DrawLine(x, y, Strings::Pad("Roll:", Strings::Format(state.flight.rollDeg,            1) + " deg"));
    DrawLine(x, y, Strings::Pad("Pitch:", Strings::Format(state.flight.pitchDeg,          1) + " deg"));
    DrawLine(x, y, Strings::Pad("Yaw:", Strings::Format(state.flight.yawDeg,              1) + " deg"));
    y -= kLineHeight / 2;
    DrawHeader(x, y, "SYSTEM");
    DrawSeparator(x, y, l, r);
    DrawLine(x, y, Strings::Pad("dt:", Strings::Format(state.dt, 4) + " s"));
}

static void DrawEnginesTab(int x, int& y, int l, int r, const AircraftState& state)
{
    for (int i = 0; i < 2; i++)
    {
        const EngineState& eng = state.engines[i];
        std::string num = std::to_string(i + 1);

        DrawHeader(x, y, "ENGINE " + num);
        DrawSeparator(x, y, l, r);
        DrawLine(x, y, Strings::Pad("N1:", Strings::Format(eng.n1, 1) + "%"));
        DrawLine(x, y, Strings::Pad("N2:", Strings::Format(eng.n2, 1) + "%"));
        DrawLine(x, y, Strings::Pad("EGT:", Strings::Format(eng.egt, 0) + " C"));
        DrawLine(x, y, Strings::Pad("Fuel Flow:", Strings::Format(eng.fuelFlow, 0) + " kg/h"));
        DrawLine(x, y, Strings::Pad("Running:", eng.running ? "YES" : "NO"));
        y -= kLineHeight / 2;
    }
}

static void DrawElectricalTab(int x, int& y, int l, int r, const AircraftState& state)
{
    DrawHeader(x, y, "ELECTRICAL");
    DrawSeparator(x, y, l, r);
    DrawLine(x, y, "No data yet");
}

static void DrawFuelTab(int x, int& y, int l, int r, const AircraftState& state)
{
    DrawHeader(x, y, "FUEL");
    DrawSeparator(x, y, l, r);
    DrawLine(x, y, "No data yet");
}

// ------------------------------------------------------
// DRAW CALLBACK
// ------------------------------------------------------ 

static void DrawDebugWindow(XPLMWindowID in_window_id, void* inRefcon)
{
    PluginState* state = static_cast<PluginState*>(inRefcon);
    if (!state) return;

    const AircraftState& aircraft = state->aircraft;
    DebugOverlayState&   debug    = state->debug;

    XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);

    int l, t, r, b;
    XPLMGetWindowGeometry(in_window_id, &l, &t, &r, &b);

    DrawTabBar(in_window_id, l, t, r, debug.activeTab);

    int x = l + kMargin;
    int y = t - kTabHeight - kMargin - kLineHeight;

    switch (debug.activeTab)
    {
        case DebugTab::Flight:     DrawFlightTab(x, y, l, r, aircraft);     break;
        case DebugTab::Engines:    DrawEnginesTab(x, y, l, r, aircraft);    break;
        case DebugTab::Electrical: DrawElectricalTab(x, y, l, r, aircraft); break;
        case DebugTab::Fuel:       DrawFuelTab(x, y, l, r, aircraft);       break;
        default: break;
    }

    const char* versionText = "v0.0.1";
    DrawLineInPlace(l + kMargin, b + kMargin, versionText);

    int textX = r - kMargin - kReloadButtonWidth;  // right justified
    int textY = b + kMargin;
    DrawLineInPlace(textX, textY, kReloadButtonText);
}

// ------------------------------------------------------
// MOUSE HANDLERS
// ------------------------------------------------------

static int HandleMouseClick(XPLMWindowID window, int x, int y, int is_down, void* inRefcon)
{
	if (!is_down) return 1;

	PluginState* state = static_cast<PluginState*>(inRefcon);
	if (!state) return 1;

	int l, t, r, b;
	XPLMGetWindowGeometry(window, &l, &t, &r, &b);

	if (y > t - kTabHeight)
	{
		int tabCount = (int)DebugTab::Count;
		int tabWidth = (r - l) / tabCount;
		int tabIndex = (x - l) / tabWidth;

		if (tabIndex >= 0 && tabIndex < tabCount)
			state->debug.activeTab = (DebugTab)tabIndex;
	}

    if (x >= r - kMargin - kReloadButtonWidth && 
        x <= r - kMargin &&
        y >= b + kMargin && 
        y <= b + kMargin + kLineHeight)
    {
        XPLMDebugString("A220-Series: Reloading plugin\n");
        XPLMReloadPlugins();
    }

	return 1;
}

// Dummy Handlers
static XPLMCursorStatus DummyHandleCursor(XPLMWindowID w, int x, int y, void* ref) { return xplm_CursorDefault; }
static int				DummyHandleWheel(XPLMWindowID w, int x, int y, int wheel, int clicks, void* ref) { return 0; }
static void				DummyHandleKey(XPLMWindowID w, char key, XPLMKeyFlags flags, char vkey, void* ref, int losing_focus) {}

// ------------------------------------------------------
// WINDOW MANAGEMENT
// ------------------------------------------------------

void RegisterDebugWindow(PluginState* state) 
{
    if (!state) return;

	int globalLeft, globalBottom, globalRight, globalTop;
	XPLMGetScreenBoundsGlobal(&globalLeft, &globalTop, &globalRight, &globalBottom);

	int screenWidth, screenHeight;
	XPLMGetScreenSize(&screenWidth, &screenHeight);

	XPLMCreateWindow_t params{};
    params.structSize           = sizeof(params);
    params.visible              = 1;
    params.drawWindowFunc       = DrawDebugWindow;
    params.handleMouseClickFunc = HandleMouseClick;
    params.handleRightClickFunc = HandleMouseClick;
    params.handleMouseWheelFunc = DummyHandleWheel;
    params.handleKeyFunc        = DummyHandleKey;
    params.handleCursorFunc     = DummyHandleCursor;
    params.refcon               = state;
    params.layer                = xplm_WindowLayerFloatingWindows;
    params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
	params.left   = globalLeft + static_cast<int>(screenWidth * 0.05f);
	params.top    = globalTop  - static_cast<int>(screenHeight * 0.1f);
	params.right  = globalLeft + static_cast<int>(screenWidth * 0.25f);
	params.bottom = globalTop  - static_cast<int>(screenHeight * 0.4f);

	XPLMWindowID window = XPLMCreateWindowEx(&params);
    state->debug.windowID = window;

	int minWindowWidth = static_cast<int>(screenWidth * 0.2f);
	int minWindowHeight = static_cast<int>(screenHeight * 0.3f);
	int maxWindowWidth = screenWidth / 2;
	int maxWindowHeight = screenHeight / 2;

	XPLMSetWindowPositioningMode(window, xplm_WindowPositionFree, -1);
	XPLMSetWindowResizingLimits(window, minWindowWidth, minWindowHeight, maxWindowWidth, maxWindowHeight);
	XPLMSetWindowTitle(window, "A220 Debug");
}

void UnregisterDebugWindow(PluginState* state)
{
    if (!state) return;

    XPLMWindowID& window = state->debug.windowID;
    if (!window) return;

	XPLMDestroyWindow(window);
	window = nullptr;
}

void ToggleDebugWindow(PluginState* state)
{
	if (!state) return;

    XPLMWindowID& window = state->debug.windowID;
    if (!window) return;

	int visible = XPLMGetWindowIsVisible(window);
	XPLMSetWindowIsVisible(window, !visible);
}
