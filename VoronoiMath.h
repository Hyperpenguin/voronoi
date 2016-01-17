#pragma once
#include <cmath>
#include "Vec2.h"
#include "QuadEdge.h"


class VoronoiMath
{
public:
	enum TriOrientation :int
	{
		CounterClockWise = 1,
		Colinear = 0,
		ClockWise = -1,
	};
	static int GetTriangleOrientation(const Vec2& a, const Vec2& b, const Vec2& c);
	inline static bool OnLeft(const Vec2& s1, const Vec2& s2, const Vec2& p) { return detTriangle(s1, s2, p) > 0; }
	inline static bool OnRight(const Vec2& s1, const Vec2& s2, const Vec2&p) { return detTriangle(s1, s2, p) < 0; }
	inline static bool Collinear(const Vec2& s1, const Vec2& s2, const Vec2&p) { return detTriangle(s1, s2, p) == 0; }
	inline static bool OnLeft(QuadEdge* q, const Vec2 p) { return OnLeft(q->Org()->p, q->Dest()->p, p); }
	inline static bool OnRight(QuadEdge* q, const Vec2 p) { return OnRight(q->Org()->p, q->Dest()->p, p); }
	inline static bool Collinear(QuadEdge* q, const Vec2 p) { return Collinear(q->Org()->p, q->Dest()->p, p); }
	// a, b, c must be counterclockwise
	static bool InCircle(const Vec2 &a, const Vec2& b, const Vec2& c, const Vec2& p);
	static float TriangleArea(const Vec2 &a, const Vec2& b, const Vec2& c);

private:
	static float detTriangle(const Vec2 &a, const Vec2& b, const Vec2& c);
};
