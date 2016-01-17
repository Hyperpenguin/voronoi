#pragma once
#include "Vertex.h"
#include "QuadEdge.h"
#include <list>
#include <forward_list>
#include <vector>
#include <deque>
#include <allocators>
#undef BOOST_HAS_THREADS
#include <boost/intrusive/list.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>

class Voronoi
{
public:
	template<typename T>
	using intrusive_member_list = boost::intrusive::list <
		T,
		boost::intrusive::member_hook < T, boost::intrusive::list_member_hook<>, &T::list_hook >
	>;

	template<typename T>
	using alloc = 
		//boost::fast_pool_allocator<T>;
		std::allocator<T>;

	typedef intrusive_member_list<Vertex> vertex_container;
	typedef intrusive_member_list<Edge> edge_container;
	typedef std::list<Vertex*> cell_container;


	Voronoi() = delete;
	Voronoi(const Rect& boundary);
	~Voronoi();

	void Clear();

	template<typename container>
	void Build(container const& points);

	template<typename container>
	void Update(container const& points) { throw "not implemented"; }


	//return nullptr if failed
	Vertex* InsertPoint(const Vec2& p);
	void RemovePoint(Vertex& v) { throw "not implemented"; }
	void MoveVertex(Vertex& v) { throw "not implemented"; }
	void ComputeVoronoi() { throw "not implemented"; }
	Vertex* LocateTriangle(const Vec2& p);
	QuadEdge* LocateTriangleEdge(const Vec2& p);
	//Vertex* LocateCell() { throw "not implemented"; }
	void ReserveMemory(int numPoints);


	inline const vertex_container& Vertices() { return vertices; }
	inline edge_container& Edges() { return edges; }
	inline const cell_container& Cells() { return cells; }

private:
	void onInit();
	void onClear();

	QuadEdge* createEdge();
	void deleteEdge(QuadEdge* e);
	void deleteEdge(Edge* e);
	Vertex* createVertex(const Vec2& p);
	void deleteVertex(Vertex* v);

	vertex_container vertices;
	intrusive_member_list<Edge> edges;
	cell_container cells;
	alloc<Vertex> vertices_alloc;
	alloc<Edge> edges_alloc;

	
	Rect boundary_rect;
	std::vector<Vec2> p_boundary;
};

template<typename container>
inline void Voronoi::Build(container const & points)
{
	Clear();
	for (const auto& p : points)
		InsertPoint(p);
}
