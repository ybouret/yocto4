#include "yocto/fame/split.hpp"
#include "yocto/core/mpi-split.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/sequence/vector.hpp"

#include <cerrno>
#include <cmath>

namespace yocto
{
    namespace fame
    {
        
        namespace
        {
            YOCTO_PAIR_DECL(Prod2D,unit_t,nx,unit_t,ny);
            inline double ratio() const throw() { return nx/double(ny); }
            YOCTO_PAIR_END();
            
            
            
            static inline Prod2D find_prod2d( const unit_t size, unit_t Nx, unit_t Ny)
            {
                vector<Prod2D> products(size,as_capacity);
                for(unit_t nx=1;nx<=size;++nx)
                {
                    for(unit_t ny=1;ny<=size;++ny)
                    {
                        if(nx*ny==size)
                        {
                            const Prod2D d(nx,ny);
                            products.push_back(d);
                        }
                    }
                }
                assert(products.size()>0);
                
                const double target = Nx/double(Ny);
                Prod2D *pmin = &products[1];
                double  dmin = fabs(target - pmin->ratio() );
                for(size_t i=2;i<=products.size();++i)
                {
                    Prod2D      &ptmp = products[i];
                    const double dtmp = fabs(target-ptmp.ratio());
                    if(dtmp<dmin)
                    {
                        dmin = dtmp;
                        pmin = &ptmp;
                    }
                }
                //std::cerr << "winner is " << *pmin << " to match " << Nx << "x" << Ny << std::endl;
                if(pmin->nx > Nx||pmin->ny > Ny)
                {
                    throw libc::exception( EDOM, "layout2D is too small");
                }
                return *pmin;
            }

            
        }
        
        
        layout2D Split(const layout2D &full_layout,
                       int     rank,
                       int     size,
                       bool    xpbc,
                       Links & xlinks,
                       bool    ypbc,
                       Links & ylinks)
        {
            //__________________________________________________________________
            //
            // Extracting full 1D layouts
            //__________________________________________________________________

            const unit_t   Nx    = full_layout.width.x;
            const unit_t   Ny    = full_layout.width.y;
            const Prod2D   P     = find_prod2d(size,Nx,Ny);
            const layout1D xfull = layout1D(full_layout.lower.x,full_layout.upper.x);
            const layout1D yfull = layout1D(full_layout.lower.y,full_layout.upper.y);

            //__________________________________________________________________
            //
            // rank = ry * nx + rx
            //__________________________________________________________________
            const int nx = P.nx;
            const int ny = P.ny;
            //std::cerr << "Decomposing " << size << " = " << nx << " x " << ny << std::endl;
            const int ry = rank / nx;
            const int rx = rank % nx;
            assert(ry*nx+rx==rank);
            //std::cerr << "rank="  << rank << " => rx=" << rx << ", ry=" << ry << std::endl;
            
            //__________________________________________________________________
            //
            // 1 D split in x, and compute effective rank
            //__________________________________________________________________

            const layout1D xl = Split(xfull,rx,nx,xpbc,xlinks);
            for(size_t i=0; i<xlinks.count; ++i)
            {
                xlinks[i].set_rank( ry * nx + xlinks[i].rank );
            }
            //std::cerr << "\txlinks=" << xlinks << std::endl;
            
            //__________________________________________________________________
            //
            // 1 D split in y, and compute effective rank
            //__________________________________________________________________
            const layout1D yl = Split(yfull,ry,ny,ypbc,ylinks);
            for(size_t i=0;i<ylinks.count;++i)
            {
                ylinks[i].set_rank( ylinks[i].rank * nx + rx );
            }
            //std::cerr << "\tylinks=" << ylinks << std::endl;

            return layout2D( coord2D(xl.lower,yl.lower), coord2D(xl.upper,yl.upper) );
        }
        
    }
    
}
