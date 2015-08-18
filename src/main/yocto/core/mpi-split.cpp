#include "yocto/core/mpi-split.hpp"

#include <cmath>

namespace yocto
{

    namespace core
    {



        static inline double ComputeH(const size_t px,
                                      const size_t py,
                                      const size_t Nx,
                                      const size_t Ny) throw()
        {
            return fabs( fabs( double(Nx)/px - double(Ny)/py) );
        }

        void find_mpi_split2D(size_t      &px,
                              size_t      &py,
                              const size_t nproc,
                              const size_t Nx,
                              const size_t Ny) throw()
        {
            assert(0==px);
            assert(0==py);
            double H    = -1;
            bool   init = true;

            for(size_t x=1;x<=nproc;++x)
            {
                for(size_t y=1;y<=nproc;++y)
                {
                    if(x*y==nproc)
                    {
                        if(init)
                        {
                            px   = x;
                            py   = y;
                            H    = ComputeH(x,y, Nx, Ny);
                            init = false;
                        }
                        else
                        {
                            const double tmp = ComputeH(x, y, Nx, Ny);
                            if(tmp<H)
                            {
                                px = x;
                                py = y;
                                H  = tmp;
                            }
                        }
                    }
                }
            }
        }
        
    }
    
}