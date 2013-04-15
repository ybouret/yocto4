#ifndef YOCTO_SPADE_OPS_LAPLACIAN_INCLUDED
#define YOCTO_SPADE_OPS_LAPLACIAN_INCLUDED 1

#include "yocto/spade/array3d.hpp"

namespace yocto
{
    namespace spade
    {
        template <typename T>
        struct laplacian
        {
            static inline
            void compute( array1D<T> &L, const array1D<T> &U, const layout1D &zone, const T inv_dx2 ) throw()
            {
                for( unit_t i=zone.lower,im=i-1,ip=i+1;i<=zone.upper;++i,++im,++ip )
                {
                    const T U0 = U[i];
                    const T mid = -(U0+U0);
                    L[i]        = inv_dx2 * ( U[im] + mid + U[ip]);
                }
            }
            
            static inline
            void compute( array2D<T> &L, const array2D<T> &U, const layout2D &zone, const math::v2d<T> &inv_dsq ) throw()
            {
                
                for( unit_t j=zone.lower.y,jm=j-1,jp=j+1;j<=zone.upper.y;++j,++jm,++jp )
                {
                    const array1D<T> &U_j  = U[j];
                    array1D<T>       &L_j  = L[j];
                    const array1D<T> &U_jm = U[jm];
                    const array1D<T> &U_jp = U[jp];
                    for( unit_t i=zone.lower.x,im=i-1,ip=i+1;i<=zone.upper.x;++i,++im,++ip )
                    {
                        const T U0  = U_j[i];
                        const T mid = -(U0+U0);
                        L_j[i]      =
                        inv_dsq.x * ( U_j[im] + mid + U_j[ip]) +
                        inv_dsq.y * ( U_jm[i] + mid + U_jp[i]);
                    }
                }
            }
            
            static inline
            void compute( array3D<T> &L, const array3D<T> &U, const layout3D &zone, const math::v3d<T> &inv_dsq ) throw()
            {
                for( unit_t k=zone.lower.z,km=k-1,kp=k+1;k<=zone.upper.z;++k,++km,++kp )
                {
                    const array2D<T> &U_k  = U[k];
                    array2D<T>       &L_k  = L[k];
                    const array2D<T> &U_km = U[km];
                    const array2D<T> &U_kp = U[kp];
                    
                    for( unit_t j=zone.lower.y,jm=j-1,jp=j+1;j<=zone.upper.y;++j,++jm,++jp )
                    {
                        array1D<T>       &L_kj  = L_k[j];
                        const array1D<T> &U_kj  = U_k[j];
                        const array1D<T> &U_kjm = U_k[jm];
                        const array1D<T> &U_kjp = U_k[jp];
                        const array1D<T> &U_kmj = U_km[j];
                        const array1D<T> &U_kpj = U_kp[j];

                        for( unit_t i=zone.lower.x,im=i-1,ip=i+1;i<=zone.upper.x;++i,++im,++ip )
                        {
                            const T U0  = U_kj[i];
                            const T mid = -(U0+U0);
                            L_kj[i]  =
                            inv_dsq.x * ( U_kj[im] + mid + U_kj[ip]) +
                            inv_dsq.y * ( U_kjm[i] + mid + U_kjp[i]) +
                            inv_dsq.z * ( U_kmj[i] + mid + U_kpj[i]);
                        }
                    }
                }
            }
            
        };
    }
    
}

#endif

