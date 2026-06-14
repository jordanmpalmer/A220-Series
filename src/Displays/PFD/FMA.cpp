#include "Core/Common.h"
#include "Displays/PFD/FMA.h"

#include "Core/State/GuidanceModeState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/PFD/Theme.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include "XPLMGraphics.h"

namespace PFD
{
    constexpr float kCenterColFrac = 0.125f;   // narrow centre column, as a fraction of frame width

	static void DrawModeCell(const Rect& frame, const char* mode, const float color[3])
	{
		FillRectColor(frame, color, kOverlayFillAlpha);
		//DrawText(cell.midX(), cell.midY(), mode, color);  // the text, centred in the cell
	}

    void DrawFMA(const Rect& frame, const GuidanceMode& modes)
    {
        const float fullW = frame.right - frame.left;
        const float centerW = kCenterColFrac * fullW;
        const float sideW = (fullW - centerW) / 4.0f;

        const float x0 = frame.left;
        const float x1 = x0 + sideW;
        const float x2 = x1 + sideW;
        const float x3 = x2 + centerW;
        const float x4 = x3 + sideW;

        const Rect atCol   = { x0, x1,          frame.top, frame.bottom };
        const Rect latCol  = { x1, x2,          frame.top, frame.bottom };
        const Rect apCol   = { x2, x3,          frame.top, frame.bottom };
        const Rect vCol    = { x3, x4,          frame.top, frame.bottom };
        const Rect altnCol = { x4, frame.right, frame.top, frame.bottom };

        float lineThickness = 0.003f * frame.width();
        const float botGap  = 0.05f * frame.height();

        DrawLineColor(Point{ x1, frame.top }, Point{ x1, frame.bottom + botGap }, lineThickness, Colors::White);
        DrawLineColor(Point{ x2, frame.top }, Point{ x2, frame.bottom + botGap }, lineThickness, Colors::White);
        DrawLineColor(Point{ x3, frame.top }, Point{ x3, frame.bottom + botGap }, lineThickness, Colors::White);
        DrawLineColor(Point{ x4, frame.top }, Point{ x4, frame.bottom + botGap }, lineThickness, Colors::White);

        DrawModeCell(atCol,   modes.autothrottle, Colors::Black);
        DrawModeCell(latCol,  modes.lateral,      Colors::Black);
        DrawModeCell(apCol,   modes.vertical,     Colors::Black);
        DrawModeCell(vCol,    modes.vertical,     Colors::Black);
        DrawModeCell(altnCol, modes.vertical,     Colors::Black);
    }
}