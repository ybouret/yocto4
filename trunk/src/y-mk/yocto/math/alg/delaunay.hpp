#ifndef YOCTO_MATH_ALG_DELAUNAY_INCLUDED
#define YOCTO_MATH_ALG_DELAUNAY_INCLUDED 1

#include "yocto/geom/v3d.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/container/sequence.hpp"

namespace yocto
{
    
    namespace math
    {
        

       
        class iTriangle
        {
        public:
            const size_t p1, p2, p3;
            
            inline iTriangle(size_t a, size_t b, size_t c) throw() :
            p1(a), p2(b), p3(c)
            {
                assert(a!=b);
                assert(b!=c);
                assert(a!=c);
            }
            
            inline ~iTriangle() throw() {}
            
            inline iTriangle( const iTriangle &t) throw() :
            p1(t.p1),p2(t.p2),p3(t.p3) {}
            
        private:
            YOCTO_DISABLE_ASSIGN(iTriangle);
        };
        
        template <typename T>
        struct delaunay
        {
            typedef geom::v2d<T> vtx2d;
            typedef geom::v3d<T> vtx3d;
            
            static
            void build( sequence<iTriangle> &trlist, const array<vtx2d> &);
            
            static
            void build( sequence<iTriangle> &trlist, const array<vtx3d> &);
            
        };
        
    }
    
}

YOCTO_SUPPORT_C_STYLE_OPS(math::iTriangle);

#endif

