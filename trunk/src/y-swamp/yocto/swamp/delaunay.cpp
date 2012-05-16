#include "yocto/swamp/delaunay.hpp"
#include "yocto/code/gsort.hpp"
#include "yocto/geom/m2d.hpp"

namespace yocto 
{
    namespace swamp 
    {
        iEdge:: iEdge( size_t a, size_t b ) throw() :
        p0(a),
        p1(b)
        {
            if( b < a )
            {
                (size_t&)p0 = b;
                (size_t&)p1 = a;
            }
        }
        
        iEdge:: ~iEdge() throw() {}
        
        iEdge:: iEdge( const iEdge &other ) throw() :
        p0( other.p0 ),
        p1( other.p1 )
        {
        }
        
        bool operator==( const iEdge &l, const iEdge &r) throw()
        {
            return (l.p0 == r.p0) and (l.p1 == r.p1);
        }
        
        static inline size_t __isort( size_t &a, size_t &b, size_t &c )
        {
            assert(a!=b);
            assert(b!=c);
            assert(a!=c);
            size_t arr[3] = { a, b, c };
            c_sort(arr, 3);
            a = arr[0];
            b = arr[1];
            c = arr[2];
            return a;
        }
        
        iTriangle:: iTriangle( size_t a, size_t b, size_t c ) throw() :
        i0( __isort(a,b,c) ),
        i1( b ),
        i2( c ),
        e0(i0,i1),
        e1(i1,i2),
        e2(i2,i0)
        {
        }
        
        iTriangle:: ~iTriangle() throw() 
        {
        }
        
        iTriangle:: iTriangle( const iTriangle &other ) throw() :
        i0( other.i0 ),
        i1( other.i1 ),
        i2( other.i2 ),
        e0( other.e0 ),
        e1( other.e1 ),
        e2( other.e2 )
        {
        }
        

        
        
    }
}

#include "yocto/math/types.hpp"
using namespace yocto::math;

#define ZTYPE float
#include "delaunay.cxx"

#undef ZTYPE
#define ZTYPE double
#include "delaunay.cxx"

