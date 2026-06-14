#pragma once

#include "Displays/Common/Geometry.h"   // Rect

void DrawLine(Point p1, Point p2, float width, const float c[3], float a = 1.0f);

void FillRect(const Rect& r);
void FillRectPoints(float l, float r, float t, float b);
void FillRectColor(const Rect& r, const float c[3], float a);

void FillPoly(const Point* pts, int count, const float c[3], float a = 1.0f);
void StrokePoly(const Point* pts, int count, bool closed, float width, const float c[3], float a = 1.0f);

template <std::size_t N>
void FillPoly(const std::array<Point, N>& pts, const float c[3], float a = 1.0f) 
{ 
	FillPoly(pts.data(), static_cast<int>(N), c, a);
}

template <std::size_t N>
void StrokePoly(const std::array<Point, N>& pts, bool closed, float width, const float c[3], float a = 1.0f)
{
	StrokePoly(pts.data(), static_cast<int>(N), closed, width, c, a);
}

