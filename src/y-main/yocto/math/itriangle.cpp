#include "yocto/math/itriangle.hpp"
#include "yocto/code/gsort.hpp"

namespace yocto
{
    
    namespace math
    {
        
        iEdge:: iEdge( size_t a, size_t b ) throw() :
        i1(a),
        i2(b)
        {
            if( i1 > i2 )
                cswap_const(i1, i2);
            assert(i1<i2);
        }
        
        iEdge:: iEdge(size_t a, size_t b, void *) throw() :
        i1(a),
        i2(b)
        {
            assert(i1<i2);
        }
        
        iEdge:: ~iEdge() throw() {}
        
        iEdge:: iEdge( const iEdge &edge ) throw() : 
        i1( edge.i1 ),
        i2( edge.i2 )
        {
        }
        
        bool operator==( const iEdge &lhs, const iEdge &rhs) throw()
        {
            return lhs.i1 == rhs.i1 && lhs.i2 == rhs.i2;
        }
        
        
        bool operator!=( const iEdge &lhs, const iEdge &rhs) throw()
        {
            return lhs.i1 != rhs.i1 || lhs.i2 != rhs.i2;
        }

        iTriangle:: iTriangle(size_t a, size_t b, size_t c) throw() :
        i1(a),
        i2(b),
        i3(c),
        next(NULL),
        prev(NULL)
        {
            gsort<size_t>( (size_t *)&i1, 3);
            assert(i1<i2);
            assert(i2<i3);
        }
        
        iTriangle:: ~iTriangle() throw() {}
        
#if 0
        iTriangle:: iTriangle(const iTriangle &tr) throw() :
        i1(tr.i1),
        i2(tr.i2),
        i3(tr.i3)
        {
            assert(i1<i2);
            assert(i2<i3);
        }
#endif
        
        static inline
        void __insert_edge( const iEdge &edge, iEdge::Buffer &edges )
        {
            for(size_t i = edges.size(); i>0 ; --i )
            {
                if( edge == edges[i] ) return;
            }
            edges.push_back( edge );
        }
        
        void iTriangle:: insert_into(  iEdge::Buffer &edges ) const
        {
            { const iEdge edge(i1,i2,0); __insert_edge(edge,edges); }
            { const iEdge edge(i2,i3,0); __insert_edge(edge,edges); }
            { const iEdge edge(i1,i3,0); __insert_edge(edge,edges); }
        }
        
        iTriangle:: List:: List() throw() {}
        iTriangle:: List:: ~List() throw() { kill(); }
        void iTriangle:: List:: kill() throw()
        {
            while(size) delete pop_back();
        }
        
        
    }

}
