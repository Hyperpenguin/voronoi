#pragma once
#include "Vec2.h"
#include <boost/intrusive/list_hook.hpp>
class QuadEdge;

//represents vertex and cell data
class Vertex
{
public:
	Vertex() :p(Vec2()) {}
	Vertex(const Vec2& _p) :p(_p) {}

	Vertex(const Vertex& v) = delete;
	Vertex& operator=(const Vertex& rhs) = delete;

	enum type :int
	{
		type_delaunay = 0,
		type_voronoi = 1,
	};

	Vec2 p;
	QuadEdge* e = nullptr;
	int flags = 0;

	boost::intrusive::list_member_hook<> list_hook;

};
