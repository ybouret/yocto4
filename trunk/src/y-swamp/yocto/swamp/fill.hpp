#ifndef YOCTO_SWAMP_FILL_INCLUDED
#define YOCTO_SWAMP_FILL_INCLUDED 1

#include "yocto/swamp/array3d.hpp"
#include "yocto/swamp/regions.hpp"

namespace  yocto  {
    
    namespace swamp
    {
        
        template <
        typename T,
        typename U>
        struct fill
        {
            YOCTO_ARGUMENTS_DECL_T;
            
            typedef functor<type,TL1(U)> function1D;
            typedef array1D<U>           axis_type;
            
            static inline void in( array1D<T> &A, const layout1D &sub, function1D &f,   const axis_type &X )
            {
                assert( X.contains(sub) );
                assert( A.contains(sub) );
                for( unit_t x=sub.lower; x<=sub.upper; ++x )
                {
                    A[x] = f(X[x]);
                }
            }
            
            typedef functor<type,TL2(U,U)> function2D;
            
            static inline void in( array2D<T> &A, const layout2D &sub, function2D &f, const axis_type &X, const axis_type &Y )
            {
                assert( A.contains(sub) );
                for(unit_t y=sub.lower.y; y<=sub.upper.y;++y)
                {
                    const U      _Y = Y[y];
                    array1D<T> & _R = A[y];
                    for( unit_t x=sub.lower.x; x<=sub.upper.x; ++x )
                    {
                        _R[x] = f(X[x],_Y);
                    }
                }
            }
            
            
        };
        
        
    }
    
}

#endif
