#ifndef YOCTO_MATH_ALG_DELAUNAY_INCLUDED
#define YOCTO_MATH_ALG_DELAUNAY_INCLUDED 1

#include "yocto/math/point3d.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/container/sequence.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    
    namespace math
    {
        
        //! triangle by indidces
        class iTriangle
        {
        public:
            const size_t p1, p2, p3;
            
            inline iTriangle(size_t a, size_t b, size_t c) throw() :
            p1(a), p2(b), p3(c)
            {
                assert(p1!=p2);
                assert(p1!=p3);
                assert(p2!=p3);
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
            typedef point2d<T> vtx2d;
            typedef point3d<T> vtx3d;
            
            //! return a list of clockwise triangles
            static
            void build( sequence<iTriangle> &trlist, const array<vtx2d> &);
            
            //! return a list of clockwise triangles
            /**
             the third dimension is untouched.
             */
            static
            void build( sequence<iTriangle> &trlist, const array<vtx3d> &);
            
            static
            T area( const array<size_t> &h, const array<vtx2d> &v);
            
            static
            T area( const array<size_t> &h, const array<vtx3d> &v);
            
            static
            void save_vtk(const string           &filename,
                          const string           &title,
                          const array<iTriangle> &tr,
                          const array<vtx2d>     &vtx);
            
            static
            void save_vtk(const string           &filename,
                          const string           &title,
                          const array<iTriangle> &tr,
                          const array<vtx3d>     &vtx);
            
        };
        
        //! return the list of indices composing the convex hull
        void delaunay_hull(sequence<size_t>          &h,
                           const array<iTriangle>    &trlist);
        
        
        
        
    }
    
}

YOCTO_SUPPORT_C_STYLE_OPS(math::iTriangle);

#endif

