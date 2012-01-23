#ifndef YOCTO_CLIFF_LAPLACIAN_INCLUDED
#define YOCTO_CLIFF_LAPLACIAN_INCLUDED 1

#include "yocto/cliff/array3d.hpp"

namespace yocto
{
	namespace cliff
	{
		
		//! compute laplacians
		template <typename T, typename U>
		struct laplacian
		{
			YOCTO_ARGUMENTS_DECL_T;
			static inline void compute( array1D<T> &lap, param_type fac, const array1D<T> &src, const U idx2, const layout1D &inside ) throw()
			{
				const_type xfac = idx2 * fac;
				for( unit_t x = inside.lower; x <= inside.upper; ++x )
				{
					const T mid( src[x] );
					lap[x] = xfac * ( src[x-1] - (mid+mid) + src[x+1] );  
				}
			}
			
			static inline void compute( array2D<T> &lap, param_type fac, const array2D<T> &src, const typename vertex2D<U>::type &idsq, const layout2D &inside ) throw()
			{
				const_type xfac = idsq.x * fac;
				const_type yfac = idsq.y * fac;
				for( unit_t y = inside.lower.y; y <= inside.upper.y; ++y )
				{
					const array1D<T> &Sm = src[y-1];
					const array1D<T> &S0 = src[y];
					const array1D<T> &Sp = src[y+1];
					array1D<T>       &L  = lap[y];
					for( unit_t x=inside.lower.x; x <= inside.upper.x; ++x )
					{
						const T mid( S0[x] );
						const T two_mid( mid+mid);
						L[x] = xfac * ( S0[x-1] - two_mid + S0[x+1] ) + yfac * ( Sm[x] - two_mid + Sp[x] );
					}
				}
			}
			
			static inline void compute( array3D<T> &lap, param_type fac, const array3D<T> &src, const typename vertex3D<U>::type &idsq, const layout3D &inside ) throw()
			{
				const_type xfac = idsq.x * fac;
				const_type yfac = idsq.y * fac;
				const_type zfac = idsq.z * fac;
				for( unit_t z = inside.lower.z; z <= inside.upper.z; ++z )
				{
					const array2D<T> &s_z  = src[z];
					const array2D<T> &s_zp = src[z+1];
					const array2D<T> &s_zm = src[z-1];
					array2D<T>       &l_z  = lap[z];
					for( unit_t y = inside.lower.y; y <= inside.upper.y; ++y )
					{						
						const array1D<T> &r_zy  = s_z[y];
						const array1D<T> &r_zym = s_z[y-1];
						const array1D<T> &r_zyp = s_z[y+1];
						const array1D<T> &r_zmy = s_zm[y];
						const array1D<T> &r_zpy = s_zp[y];
						array1D<T>       &l_zy  = l_z[y];
						for( unit_t x=inside.lower.x; x <= inside.upper.x; ++x )
						{
							const T mid( r_zy[x] );
							const T two_mid( mid + mid );
							l_zy[x] =
							xfac * ( r_zy[x-1] - two_mid + r_zy[x+1] ) +
							yfac * ( r_zym[x]  - two_mid + r_zyp[x]  ) +
							zfac * ( r_zmy[x]  - two_mid + r_zpy[x]  );
						}
					}
				}
			}
			
			
		};
		
	}
	
}

#endif
