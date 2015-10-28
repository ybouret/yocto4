#ifndef YOCTO_PARALLEL_COORD_INCLUDED
#define YOCTO_PARALLEL_COORD_INCLUDED 1

#include "yocto/math/point3d.hpp"

namespace yocto
{
    namespace parallel
    {

        typedef unit_t          coord1D; //!< 1D logical coordinate
        typedef point2d<unit_t> coord2D; //!< 2D logical coordinate
        typedef point3d<unit_t> coord3D; //!< 3D logical coordinate

        
        //! extract coord from compound COORD
        template <typename COORD>
        inline unit_t & __coord( COORD &C, size_t dim ) throw()
        {
            assert(dim<sizeof(COORD)/sizeof(unit_t));
            return *(((unit_t *)&C)+dim);
        }

        //! extract coord from compound COORD (const)
        template <typename COORD>
        inline const unit_t & __coord( const COORD &C, size_t dim ) throw()
        {
            assert(dim<sizeof(COORD)/sizeof(unit_t));
            return *(((unit_t *)&C)+dim);
        }

        //! decrease all coordinates
        template <typename COORD>
        inline COORD __coord_dec(COORD C) throw()
        {
            unit_t *q = (unit_t *)&C;
            for(size_t i=0;i<sizeof(COORD)/sizeof(unit_t);++i)
            {
                --q[i];
            }
            return C;
        }

        //! compare coordinates (integral types)
        template <typename COORD>
        inline bool are_same_coord( const COORD &A, const COORD &B ) throw()
        {
            const unit_t * a = & __coord(A,0);
            const unit_t * b = & __coord(B,0);
            for(size_t i=0;i<sizeof(COORD)/sizeof(unit_t);++i)
            {
                if(a[i]!=b[i])
                    return false;
            }
            return true;
        }
        
        
    }
}

#endif

