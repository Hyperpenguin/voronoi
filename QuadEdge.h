#pragma once
#include "Vertex.h"
#include "boost/intrusive/list_hook.hpp"

class Edge;

class QuadEdge
{
	friend class Edge;

private:
	QuadEdge() {}

	QuadEdge(const QuadEdge& other) = delete;
	void operator=(const QuadEdge& other) = delete;

public:
	inline QuadEdge* Rot()  { return index < 3 ? this + 1 : this - 3; }
	inline QuadEdge* Sym()  { return index < 2 ? this + 2 : this - 2; }
	inline QuadEdge* InvRot()  { return index < 1 ? this + 3 : this - 1; }
	inline QuadEdge* Rnext()  { return Rot()->oNext->InvRot(); }
	inline QuadEdge* Lnext()  { return InvRot()->oNext->Rot(); }
	inline QuadEdge* Onext()  { return oNext; }
	inline QuadEdge* Dnext()  { return Sym()->oNext->Sym(); }
	inline QuadEdge* Rprev()  { return Sym()->oNext; }
	inline QuadEdge* Lprev()  { return oNext->Sym(); }
	inline QuadEdge* Oprev()  { return Rot()->oNext->Rot(); }
	inline QuadEdge* Dprev()  { return InvRot()->oNext->InvRot(); }
	inline Vertex* Org()  { return v; }
	inline Vertex* Dest()  { return Sym()->v; }
	inline Vertex* Right()  { return Rot()->v; }
	inline Vertex* Left()  { return InvRot()->v; }
	inline QuadEdge* RootQuadEdge() { return (this - index); }
	inline Edge* RootEdge(); 
	inline void SetOrg(Vertex* org) { v = org; }
	inline void SetDest(Vertex* dest) { Sym()->v = dest; }
	inline void SetEndPoints(Vertex* org, Vertex* dest) { SetOrg(org); SetDest(dest); }

	static void Connect(QuadEdge* e, QuadEdge* eLprev, QuadEdge* eLnext);
	static void Disconnect(QuadEdge* e);
	static void Splice(QuadEdge* ea, QuadEdge* eb);
	static void Flip(QuadEdge* e);


private:
	//used for finding rot, sym, invrot
	int index;

	QuadEdge* oNext;
	Vertex* v=nullptr;
};

class Edge
{
public:
	Edge();

	inline Vertex* Org()  { return e[0].Org(); }
	inline Vertex* Dest()  { return e[2].Org(); }

	QuadEdge e[4];
	boost::intrusive::list_member_hook<> list_hook;

};



Edge* QuadEdge::RootEdge() 
{ 
	return reinterpret_cast<Edge*>(
		reinterpret_cast<int8_t*>(this) - offsetof(Edge, e[index])
		);
}