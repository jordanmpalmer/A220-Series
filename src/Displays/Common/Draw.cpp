#include "Core/Common.h"
#include "Displays/Common/Draw.h"

#include "Displays/Common/Geometry.h"

void DrawLine(Point p1, Point p2, float width)
{
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y); glVertex2f(p2.x, p2.y);
	glEnd();
}

void DrawLineColor(Point p1, Point p2, float width, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], 1.0f);
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y); glVertex2f(p2.x, p2.y);
	glEnd();
}


void FillRect(const Rect& r)
{
    glBegin(GL_QUADS);
		glVertex2f(r.left, r.bottom);
		glVertex2f(r.left, r.top);
		glVertex2f(r.right, r.top);
		glVertex2f(r.right, r.bottom);
	glEnd();
}

void FillRectPoints(float l, float r, float t, float b)
{
    glBegin(GL_QUADS);
		glVertex2f(l, b);
		glVertex2f(l, t);
		glVertex2f(r, t);
		glVertex2f(r, b);
	glEnd();
}

void FillRectColor(const Rect& r, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], a);
	FillRect(r);
}

void FillPolyColor(const Point* pts, int count, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], a);
	glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < count; ++i) glVertex2f(pts[i].x, pts[i].y);
	glEnd();
}

void StrokePolyColor(const Point* pts, int count, bool closed, float width, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], a);
	glLineWidth(width);
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
		for (int i = 0; i < count; ++i) glVertex2f(pts[i].x, pts[i].y);
	glEnd();
}


// ------------------------------------------------------
// CORNER CAPS
// ------------------------------------------------------

//// Fills the gap between a square corner and a rounded arc, in black.
//// (px, py)   = the actual sharp rectangle corner being hidden
//// (ax, ay)   = the center the arc sweeps around (inset by radius from the corner)
//// startAngle = radians; which 90 degree quadrant this corner occupies
//static void DrawCornerCap(float px, float py, float ax, float ay,
//    float radius, float startAngle)
//{
//    const int   segments = 8;
//    const float step = kHalfPi / segments;
//
//    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
//
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(px, py);                          // fan anchored at the sharp corner
//    for (int i = 0; i <= segments; ++i)          // sweep the arc
//    {
//        float a = startAngle + step * i;
//        glVertex2f(ax + radius * std::cos(a),
//            ay + radius * std::sin(a));
//    }
//    glEnd();
//}

//// Caps all four corners of the given rectangle so it appears rounded.
//static void DrawRoundedCorners(float left, float bottom, float right, float top, float radius)
//{
//    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//
//    // Top-left      90 -> 180
//    DrawCornerCap(left, top, left + radius, top - radius, radius, kHalfPi);
//    // Top-right      0 -> 90
//    DrawCornerCap(right, top, right - radius, top - radius, radius, 0.0f);
//    // Bottom-left  180 -> 270
//    DrawCornerCap(left, bottom, left + radius, bottom + radius, radius, kPi);
//    // Bottom-right 270 -> 360
//    DrawCornerCap(right, bottom, right - radius, bottom + radius, radius, 3.0f * kHalfPi);
//}


