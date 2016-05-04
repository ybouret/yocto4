#ifndef YOCTO_MATH_LINEAR_INCLUDED
#define YOCTO_MATH_LINEAR_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/math/point2d.hpp"

namespace yocto 
{
    
    namespace math
    {
        //! assume X is ordered
        /**
         Find a linear interpolation
         */
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
        /**
         Find a linear interpolation
         */
        template<typename T>
        inline T linear(  T x, const array< point2d<T> > &V )
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
        
        //! find all positions matching one value
        template <typename T>
        inline void linear_find( T y, sequence<T> &x, const array<T> &X, const array<T> &Y, const size_t nmax = 0 )
        {
            assert( X.size() == Y.size() );
            const size_t n = X.size();
            x.free();
            for( size_t i=1; i < n; ++i )
            {
                const T Y1 = Y[i];
                const T Y2 = Y[i+1];
                if( (y-Y1) * (y-Y2) <= 0 )
                {
                    x.push_back(X[i] + (X[i+1] - X[i]) * (y-Y1) / (Y2-Y1));
                    if( nmax > 0 && x.size() >= nmax ) return;
                }
            }
        }
    }
    
}

#endif
