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
            
            const unit_t Nx = full.width.x;
            const unit_t Ny = full.width.y;
            const unit_t Nz = full.width.z;
            
            find_size(sizes,Nx,Ny,Nz,size);
            std::cerr << "best sizes="  << sizes << std::endl;
            
            return full;
        }
        
    }
}
