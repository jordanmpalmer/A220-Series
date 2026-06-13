#include "Core/Common.h"
#include "Displays/PFD/FMA.h"

#include "Core/State/GuidanceModeState.h"
#include "Displays/PFD/Layout.h"
#include "Displays/Common/Draw.h"
#include "Displays/Common/Colors.h"

#include "XPLMGraphics.h"

namespace PFD
{
	static void DrawModeCell(const Rect& cell, const char* mode, const float color[3])
	{
		FillRect(cell);                                   // the box (outline/background)
		//DrawText(cell.midX(), cell.midY(), mode, color);  // the text, centred in the cell
	}

	void DrawFMA(const Rect& frame, const GuidanceMode& modes) // will need to be swapped to GuidanceMode struct
	{
		XPLMSetGraphicsState(0, 0, 0, 0, 1, 0, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

		glColor4f(1.0f, 0.0f, 0.0f, 0.3f);

		const float colW         = (frame.right - frame.left) / 5.0f;
		const float topBarBottom = frame.top - 0.68f * frame.height();
		const float boxTops      = frame.bottom + 0.25f * frame.height();

		const Rect atCol =    { frame.left,                          frame.left + colW,                  frame.top, topBarBottom };
		const Rect vCol =     { frame.left + colW,                   frame.left + 2 * colW,              frame.top, topBarBottom };
		const Rect latCol =   { frame.left + 2 * colW,               frame.right,                        frame.top, topBarBottom };
		const Rect speedBox = { frame.left,                          frame.left + 0.119f * frame.width(), boxTops,   frame.bottom};
		const Rect altBox =   { frame.right - 0.154f * frame.width(), frame.right,                        boxTops,   frame.bottom};

		DrawModeCell(atCol, modes.autothrottle, Colors::kWhite);
		DrawModeCell(vCol, modes.vertical, Colors::kGreen);
		DrawModeCell(latCol, modes.lateral, Colors::kGreen);
		DrawModeCell(speedBox, modes.lateral, Colors::kMagenta);
		DrawModeCell(altBox, modes.lateral, Colors::kCyan);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}