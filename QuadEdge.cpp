#include "QuadEdge.h"
#include <array>
#include <utility>




void QuadEdge::Connect(QuadEdge * e, QuadEdge * eLprev, QuadEdge * eLnext)
{
	QuadEdge::Splice(e, eLprev->Lnext());
	QuadEdge::Splice(e->Sym(), eLnext);
	e->SetEndPoints(eLprev->Dest(), eLnext->Org());
}

void QuadEdge::Disconnect(QuadEdge * e)
{
	Splice(e, e->Oprev());
	Splice(e->Sym(), e->Sym()->Oprev());
}


void QuadEdge::Splice(QuadEdge * ea, QuadEdge * eb)
{
	QuadEdge* ex = ea->Onext()->Rot();
	QuadEdge* ey = eb->Onext()->Rot();

	QuadEdge* et1 = ea->oNext;
	QuadEdge* et2 = eb->oNext;
	QuadEdge* et3 = ex->oNext;
	QuadEdge* et4 = ey->oNext;
	
	ea->oNext = et2;
	eb->oNext = et1;
	ex->oNext = et4;
	ey->oNext = et3;
}

void QuadEdge::Flip(QuadEdge * e)
{
	QuadEdge* a = e->Oprev();
	QuadEdge* b = e->Sym()->Oprev();
	Splice(e, a);
	Splice(e->Sym(), b);
	Splice(e, a->Lnext());
	Splice(e->Sym(), b->Lnext());
	e->SetEndPoints(a->Dest(), b->Dest());
}

Edge::Edge()
{
	for (int i = 0; i < 4; i++)
		e[i].index = i;

	e[0].oNext = &(e[0]);
	e[1].oNext = &(e[3]);
	e[2].oNext = &(e[2]);
	e[3].oNext = &(e[1]);
}
