#ifndef YOCTO_CLIFF_FILL_INCLUDED
#define YOCTO_CLIFF_FILL_INCLUDED 1

#include "yocto/cliff/array3d.hpp"


namespace yocto
{
	
	namespace cliff
	{
		
		template <typename T, typename U>
		struct fill
		{
			typedef functor<T,TL1(U)> function1;
			
			static inline void with( function1 &f, array1D<T> &a, const layout1D &sub, const array1D<U> &X )
			{
				for( unit_t x=sub.lower; x<= sub.upper; ++x )
				{
					a[x] = f( X[x] );
				}
			}
			
			typedef functor<T,TL2(U,U)> function2;
			
			static inline void with( function2 &f, array2D<T> &a, const layout2D &sub, const array1D<U> &X, const array1D<U> &Y )
			{
				for( unit_t y=sub.lower.y; y <= sub.upper.y; ++y )
				{
					const U      _Y = Y[y];
					array1D<T> &a_y = a[y];
					for( unit_t x=sub.lower.x; x<= sub.upper.x; ++x )
					{
						a_y[x] = f( X[x], _Y );
					}
				}
			}
			
			typedef functor<T,TL3(U,U,U)> function3;
			
			static inline void with( function3 &f, array3D<T> &a, const layout3D &sub, const array1D<U> &X, const array1D<U> &Y, const array1D<U> &Z )
			{
				for( unit_t z=sub.lower.z; z <= sub.lower.z; ++z )
				{
					const U      _Z = Z[z];
					array2D<T> &a_z = a[z];
					for( unit_t y=sub.lower.y; y <= sub.upper.y; ++y )
					{
						const U      _Y  = Y[y];
						array1D<T> &a_zy = a_z[y];
						for( unit_t x=sub.lower.x; x<= sub.upper.x; ++x )
						{
							a_zy[x] = f( X[x], _Y, _Z );
						}
					}
				}
			}
			
		};
		
	}
	
}

#endif
