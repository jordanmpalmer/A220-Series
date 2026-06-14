#include "Core/Common.h"
#include "Displays/Common/Draw.h"

#include "Displays/Common/Geometry.h"

void DrawLine(Point p1, Point p2, float width, const float c[3], float a)
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

void FillPoly(const Point* pts, int count, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], a);
	glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < count; ++i) glVertex2f(pts[i].x, pts[i].y);
	glEnd();
}

void StrokePoly(const Point* pts, int count, bool closed, float width, const float c[3], float a)
{
	glColor4f(c[0], c[1], c[2], a);
	glLineWidth(width);
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
		for (int i = 0; i < count; ++i) glVertex2f(pts[i].x, pts[i].y);
	glEnd();
}
