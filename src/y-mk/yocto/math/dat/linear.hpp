#ifndef YOCTO_MATH_LINEAR_INCLUDED
#define YOCTO_MATH_LINEAR_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/geom/v2d.hpp"

namespace yocto 
{
    
    namespace math
    {
        //! assume x is ordered
        template<typename T,typename U>
        inline U linear(  T x, const array<T> &X, const array<U> &Y )
        {
            assert( Y.size() == X.size() );
            assert( X.size() > 0 );
            const size_t n = X.size(); 
            if( x <= X[1] )
				return Y[1];
			else {
                
				if( x >= X[n] )
					return Y[n];
				else
				{
					size_t klo = 1, khi = n;
					while( (khi-klo)> 1 )
					{
						const size_t k = (klo+khi)>>1;
						if( X[k] > x )
							khi = k;
						else
							klo = k;
					}
                    return Y[klo] + (x-X[klo]) * (Y[khi] - Y[klo]) / (X[khi] - X[klo]);
				}
			}
        }
        
        //! assume order by x
        template<typename T>
        inline T linear(  T x, const array< geom::v2d<T> > &V )
        {
            assert( V.size() > 0 );
            const size_t n = V.size();
            if( x <= V[1].x )
				return V[1].y;
			else {
                
				if( x >= V[n].x )
					return V[n].y;
				else
				{
					size_t klo = 1, khi = n;
					while( (khi-klo)> 1 )
					{
						const size_t k = (klo+khi)>>1;
						if( V[k].x > x )
							khi = k;
						else
							klo = k;
					}
                    return V[klo].y + (x-V[klo].x) * (V[khi].y - V[klo].y) / (V[khi].x - V[klo].x);
				}
			}

            
        }
    }
    
}

#endif
