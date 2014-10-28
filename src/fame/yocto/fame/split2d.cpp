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
            YOCTO_PAIR_DECL(Prod2D,unit_t,p,unit_t,q);
            YOCTO_PAIR_END();
            
            
            
            static inline Prod2D find_prod2d( const unit_t size, unit_t Nx, unit_t Ny)
            {
                vector<Prod2D> products(size,as_capacity);
                for(size_t p=1;p<=size;++p)
                {
                    for(size_t q=1;q<=size;++q)
                    {
                        if(p*q==size)
                        {
                            const Prod2D d(p,q);
                            products.push_back(d);
                        }
                    }
                }
                assert(products.size()>0);
                
                const double target = Nx/double(Ny);
                Prod2D *pmin = &products[1];
                double  dmin = fabs(target - pmin->p / double(pmin->q));
                for(size_t i=2;i<=products.size();++i)
                {
                    Prod2D      &ptmp = products[i];
                    const double dtmp = fabs(target-ptmp.p/double(ptmp.q));
                    if(dtmp<dmin)
                    {
                        dmin = dtmp;
                        pmin = &ptmp;
                    }
                }
                std::cerr << "winner is " << *pmin << " to match " << Nx << "x" << Ny << std::endl;
                if(pmin->p>Nx||pmin->q>Ny)
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
            
            const Prod2D P = find_prod2d(size,full_layout.width.x,full_layout.width.y);
            return full_layout;
        }
        
    }
    
}
