#include "yocto/fame/split/quad3d.hpp"
#include "yocto/fame/split/quad1d.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    using namespace math;
    
    namespace fame
    {
        
        namespace
        {
            static inline
            void find_size(coord3D     &sizes,
                           const unit_t Nx,
                           const unit_t Ny,
                           const unit_t Nz,
                           const unit_t size)
            {
                const double N2 = Nx*Nx + Ny*Ny + Nz*Nz;
                double       best = -1;
                for(unit_t i=1;i<=size;++i)
                {
                    for(unit_t j=1;j<=size;++j)
                    {
                        for(unit_t k=1;k<=size;++k)
                        {
                            
                            if(i*j*k==size)
                            {
                                const double alpha = ( (i*Nx) + (j*Ny) + (k*Nz) )/N2;
                                const double norm  =
                                sqrt(
                                     Square( double(i) - alpha * double(Nx) ) +
                                     Square( double(j) - alpha * double(Ny) ) +
                                     Square( double(k) - alpha * double(Nz) )
                                     )
                                ;
                                //std::cerr <<  "norm(" << i << "," << j << "," << k << ")=" << norm << ", alpha=" << alpha << std::endl;
                                if(best<0|| norm<best)
                                {
                                    sizes.x = i;
                                    sizes.y = j;
                                    sizes.z = k;
                                    best = norm;
                                }
                                
                            }
                            
                        }
                    }
                }
            }
            
        }
        
        layout3D quad3D:: split(const layout3D &full,
                                const int       rank,
                                const int       size,
                                const bool      xpbc,
                                quad_links     &xlinks,
                                const bool      ypbc,
                                quad_links     &ylinks,
                                const bool      zpbc,
                                quad_links     &zlinks,
                                coord3D        &ranks,
                                coord3D        &sizes)
        {
            
            assert(rank>=0);
            assert(rank<size);
            
            //__________________________________________________________________
            //
            // computing the best sizes
            //__________________________________________________________________
            const unit_t Nx = full.width.x;
            const unit_t Ny = full.width.y;
            const unit_t Nz = full.width.z;
            
            find_size(sizes,Nx,Ny,Nz,size);
            
            //__________________________________________________________________
            //
            // local ranks rank =  (ny*nx) * rz + (nx * ry) + rx
            //__________________________________________________________________
            const unit_t nx   = sizes.x;
            const unit_t ny   = sizes.y;
            const unit_t nxny = nx * ny;
            const unit_t nz   = sizes.z;
            
            const unit_t rz   = rank / nxny;
            const unit_t uy   = rank % nxny;
            const unit_t ry   = uy   / nx  ;
            const unit_t rx   = uy   % nx  ;
            
            ranks.x = rx;
            ranks.z = rz;
            ranks.y = ry;
            
            
            assert( rank == nxny * rz + nx * ry + rx);
            
            //__________________________________________________________________
            //
            // 1D X split
            //__________________________________________________________________
            const layout1D xfull(full.lower.x,full.upper.x);
            const layout1D xsub = quad1D::split(xfull, rx, nx, xpbc, xlinks);
            for(size_t i=0;i<xlinks.count;++i)
            {
                xlinks[i].rank = nxny * rz +  nx * ry + xlinks[i].rank;
            }
            
            //__________________________________________________________________
            //
            // 1D Y split
            //__________________________________________________________________
            const layout1D yfull(full.lower.y,full.upper.y);
            const layout1D ysub = quad1D::split(yfull, ry, ny, ypbc, ylinks);
            for(size_t i=0;i<ylinks.count;++i)
            {
                ylinks[i].rank = nxny*rz + nx * ylinks[i].rank + rx;
            }
            
            
            //__________________________________________________________________
            //
            // 1D Z split
            //__________________________________________________________________
            const layout1D zfull(full.lower.z,full.upper.z);
            const layout1D zsub = quad1D::split(zfull, rz, nz, zpbc, zlinks);
            for(size_t i=0;i<zlinks.count;++i)
            {
                zlinks[i].rank = nx*ny * zlinks[i].rank + nx * ry + rx;
            }
            
            return layout3D( coord3D(xsub.lower,ysub.lower,zsub.lower), coord3D(xsub.upper,ysub.upper,zsub.upper) );
        }
        
    }
}
