#include "Voronoi.h"
#include "VoronoiMath.h"
#include <array>
#include <cassert>
#include <algorithm>

//temporary
#include <cstdio>
#include <iostream>



Voronoi::Voronoi(const Rect & _boundary) 
	:boundary_rect(_boundary)
	,edges_alloc()
{
	onInit();
}

Voronoi::~Voronoi()
{
	onClear();
}

void Voronoi::Clear()
{
	onClear();
	onInit();
}

Vertex* Voronoi::InsertPoint(const Vec2& p)
{
	QuadEdge* e = LocateTriangleEdge(p);
	if (e == nullptr ||
		p == e->Org()->p || p == e->Dest()->p)
		return nullptr;
	else if (VoronoiMath::Collinear(e, p))
	{
		e = e->Oprev();
		QuadEdge* et = e->Onext();
		QuadEdge::Disconnect(et);
		deleteEdge(et->RootEdge());
	}
	Vertex* v = createVertex(p);
	v->e = e;
	
	QuadEdge* e_begin = createEdge();
	e_begin->SetEndPoints(e->Org(), v);
	QuadEdge* et = e_begin;
	QuadEdge::Splice(e_begin, e);
	do
	{
		QuadEdge* et2 = createEdge();
		QuadEdge::Connect(et2, e, et->Sym());
		et = et2;
		e = et->Oprev();
	} while (e->Lnext() != e_begin);

	do
	{
		et = e->Oprev();
		if (VoronoiMath::OnRight(e, et->Dest()->p) &&
			VoronoiMath::InCircle(e->Org()->p, et->Dest()->p, e->Dest()->p, p))
		{
			QuadEdge::Flip(e);
			e = e->Oprev();
		}
		else if (e->Onext() == e_begin)
			break;
		else
			e = e->Onext()->Lprev();
	} while (true);

	return v;
}

Vertex * Voronoi::LocateTriangle(const Vec2& p)
{
	throw "not implemented";//return LocateTriangleEdge(p)->
}

QuadEdge * Voronoi::LocateTriangleEdge(const Vec2 & p)
{
	int i = 0;
	QuadEdge* e = &edges.front().e[0];
	while (true)
	{
		if (e->Org()->p == p || e->Dest()->p == p)
			return e;

		if (VoronoiMath::OnRight(e, p))
			e = e->Sym();
		else if (!VoronoiMath::OnRight(e->Onext(), p))
			e = e->Onext();
		else if (!VoronoiMath::OnRight(e->Dprev(), p))
			e = e->Dprev();
		else
			return e;
		if (++i > 100000)
			break;
	}
	return nullptr;
}

void Voronoi::ReserveMemory(int numPoints)
{
	auto alloc_reserve = [](auto& allocator, std::size_t n)
	{
		auto p = allocator.allocate(n);
		allocator.deallocate(p, n);
	};
	
	alloc_reserve(vertices_alloc, numPoints);
	alloc_reserve(edges_alloc, numPoints * 3);
}


void Voronoi::onInit()
{
	assert((vertices.size() == 0) && (edges.size() == 0) && "container should be empty");

	// should check if p_boundary is empty.

	//create boundary
	p_boundary = { Vec2(0,0),Vec2(1,0),Vec2(1,1),Vec2(0,1) };
	for (auto& p : p_boundary)
		p = Vec2(
			((p.x - 0.5) * 10 + 0.5)*boundary_rect.width + boundary_rect.left,
			((p.y - 0.5) * 10 + 0.5)*boundary_rect.height + boundary_rect.top);

	for (int i = 0; i < p_boundary.size(); i++)
	{
		auto v = createVertex(p_boundary[i]);
	}
	auto it_back_v = std::prev(vertices.end());
	for (auto it = vertices.begin(); it != vertices.end(); it++)
	{
		QuadEdge* e = createEdge();
		e->SetEndPoints(&*it_back_v, &*it);
		(&*it_back_v)->e = &edges.back().e[0];
		it_back_v = it;
	}

	auto it_back_e = std::prev(edges.end());
	for (auto it = edges.begin(); it != edges.end(); it++)
	{
		QuadEdge::Splice((*it_back_e).e[0].Sym(), &(*it).e[0]);
		it_back_e = it;
	}

	//triangulation
	if (p_boundary.size() >= 4)
	{
		QuadEdge* e = &edges.begin()->e[0];
		QuadEdge* e_end = e->Lprev()->Lprev();
		e = e->Lnext();
		do
		{
			QuadEdge* et = e->Lnext();
			QuadEdge::Connect(createEdge(), e, e->Lprev());
			e = et;
		} while (e != e_end);
	}
}

void Voronoi::onClear()
{
	auto clearContainter = [](auto& container)
	{
		for (auto it = container.begin(); it != container.end(); it++)
			if (*it != nullptr)
				delete *it;
		container.clear();
	};

	clearContainter(cells);

	edges.clear_and_dispose(
		[&](Edge* e) 
	{ 
		//delete e; 
		edges_alloc.destroy(e);
		edges_alloc.deallocate(e, 1);
	});

	vertices.clear_and_dispose(
		[&](Vertex* v)
	{
		//delete v;
		vertices_alloc.destroy(v);
		vertices_alloc.deallocate(v,1);
	});
}

QuadEdge * Voronoi::createEdge()
{
	//boost::fast_pool_allocator<Edge> pool;
	//auto e = new Edge();
	auto e = edges_alloc.allocate(1);
	edges_alloc.construct(e); 
	edges.push_back(*e);
	return &(e->e[0]);
}

void Voronoi::deleteEdge(QuadEdge * e)
{
	deleteEdge(e->RootEdge());
}

void Voronoi::deleteEdge(Edge * e)
{
	edges.erase(edges.iterator_to(*e));
	//delete e;
	edges_alloc.destroy(e);
	edges_alloc.deallocate(e, 1);
}

Vertex * Voronoi::createVertex(const Vec2& p)
{
	//Vertex* v = new Vertex(p);
	auto v = vertices_alloc.allocate(1);
	vertices_alloc.construct(v, p);
	vertices.push_back(*v);
	return v;
}

void Voronoi::deleteVertex(Vertex* v)
{
	vertices.erase(vertices.iterator_to(*v));
	//delete v;
	vertices_alloc.destroy(v);
	vertices_alloc.deallocate(v,1);
}
