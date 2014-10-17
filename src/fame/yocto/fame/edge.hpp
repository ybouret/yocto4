#ifndef YOCTO_FAME_EDGE_INCLUDED
#define YOCTO_FAME_EDGE_INCLUDED 1

#include "yocto/fame/vertices.hpp"

namespace yocto
{
    namespace fame
    {
        template <size_t DIM,typename T>
        class Edge
        {
        public:
            YOCTO_MAKE_OBJECT
            
            typedef Vertex<DIM,T> VTX;
            typedef VTX          *PVTX;
            PVTX  v0;
            PVTX  v1;
            Edge *next;
            Edge *prev;
            
            inline  Edge(PVTX p0, PVTX p1) throw() : v0(p0),   v1(p1),   next(0), prev(0)  { assert(v0); assert(v1); assert(v0!=v1); }
            inline  Edge(const Edge &e   ) throw() : v0(e.v0), v1(e.v1), next(0), prev(0)  { assert(v0); assert(v1); assert(v0!=v1); }
            inline ~Edge() throw() {}
            inline  Edge & operator=(const Edge &e) throw() { v0=e.v0; v1=e.v1; assert(v0);assert(v1); assert(v0!=v1);}
            
            inline  bool is_equal( const Edge &e ) const throw()
            {
                return (v0 == e.v0) && (v1 == e.v1);
            }
            
            inline bool is_opposite( const Edge &e ) const throw()
            {
                return (v0==e.v1) && (v1==e.v0);
            }
            
            
        };
    }
}

#endif
