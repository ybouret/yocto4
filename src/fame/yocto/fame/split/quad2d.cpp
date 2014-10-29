#include "yocto/fame/split/quad2d.hpp"
#include "yocto/fame/split/quad1d.hpp"

#include <cmath>

namespace yocto
{
    
    namespace fame
    {
        
        static inline
        void find_sizes( coord2D &sizes, const unit_t Nx, const unit_t Ny,  const int size )
        {
            const double target = Nx/double(Ny);
            double       dopt   = -1;
            int          xopt   = -1;
            int          yopt   = -1;
            
            for(int px=1;px<=size;++px)
            {
                for(int py=1;py<=size;++py)
                {
                    if(px*py==size)
                    {
                        const double ratio = px/double(py);
                        const double dtmp  = fabs(target-ratio);
                        if(dopt<0 || dtmp<dopt)
                        {
                            xopt = px;
                            yopt = py;
                            dopt = dtmp;
                        }
                    }
                }
            }
            sizes.x = xopt;
            sizes.y = yopt;
            
        }
        
        layout2D quad_split(const layout2D &full,
                            const int       rank,
                            const int       size,
                            const bool      xpbc,
                            quad_links     &xlinks,
                            const bool      ypbc,
                            quad_links     &ylinks,
                            coord2D        &ranks,
                            coord2D        &sizes)
        {
            
            assert(size>0);
            assert(rank<size);
            
            const unit_t Nx = full.width.x;
            const unit_t Ny = full.width.y;
            
            find_sizes(sizes,Nx,Ny,size);
        
            //__________________________________________________________________
            //
            // rank = nx * ry + rx
            //__________________________________________________________________
            const int nx = sizes.x;
            const int ny = sizes.y;
            
            const int rx = rank % nx;
            const int ry = rank / nx;
            assert(nx*ry+rx==rank);
            ranks.x = rx;
            ranks.y = ry;
            
            //__________________________________________________________________
            //
            // 1D X split
            //__________________________________________________________________
            const layout1D xfull(full.lower.x,full.upper.x);
            const layout1D xsub = quad_split(xfull, rx, nx, xpbc, xlinks);
            for(size_t i=0;i<xlinks.count;++i)
            {
                xlinks[i].rank = nx * ry + xlinks[i].rank;
            }
            
            //__________________________________________________________________
            //
            // 1D Y split
            //__________________________________________________________________
            const layout1D yfull(full.lower.y,full.upper.y);
            const layout1D ysub = quad_split(yfull, ry, ny, ypbc, ylinks);
            for(size_t i=0;i<ylinks.count;++i)
            {
                ylinks[i].rank = nx * ylinks[i].rank + rx;
            }
            
            return layout2D( coord2D(xsub.lower,ysub.lower), coord2D(xsub.upper,ysub.upper) );
            
        }
        
    }
    
}


