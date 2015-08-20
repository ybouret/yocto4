#include "yocto/parallel/split.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace parallel
    {
        patch1D split::compute(size_t rank,size_t size,const patch1D &source)
        {
            unit_t offset = source.lower;
            unit_t length = source.width;
            split::in1D(rank, size, offset, length);

            if(length<=0)
                throw exception("parallel.split produced empty patch!");

            return patch1D(offset,offset+length-1);
        }

    }

}

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace parallel
    {
        split::in2D:: ~in2D() throw() {}


        void split:: in2D:: init(const size_t Lx, const size_t Ly) throw()
        {
            assert(Lx>0);
            assert(Ly>0);
            size_t &nx = (size_t &)xsize;
            size_t &ny = (size_t &)ysize;
            if(size<=1)
            {
                nx = ny =1;
            }
            else
            {
                // initializing with a linear split
                const size_t T0  = 4 * min_of(Lx,Ly);
                const size_t sLX = size * Lx;
                const size_t sLY = size * Ly;
                double       tau = -1;
                if(Lx>Ly)
                {
                    ny=1;
                    nx=size;
                    tau= double(T0+2*sLY)/size;
                }
                else
                {
                    nx=1;
                    ny=size;
                    tau= double(T0+2*sLX)/size;
                }
                std::cerr << "\t\tnx=" << nx << ", ny=" << ny << ", tau=" << tau << std::endl;
                // trying sub
                for(size_t i=2;i<=size;++i)
                {
                    for(size_t j=2;j<=size;++j)
                    {
                        if(i*j==size)
                        {
                            const double tmp = ( double(T0) + 2.0 * ( double(sLX)/i + double(sLY)/j ) )/size;
                            std::cerr << "\t\ti=" << i << ", j=" << j << ", tmp=" << tmp << std::endl;

                        }
                    }
                }

            }
        }

#define Y_PARA_SPLIT2D_INIT() \
size( nproc <= 0 ? 1 : nproc ), \
xsize(0), \
ysize(0)

        split:: in2D:: in2D(const size_t nproc, const size_t Nx, const size_t Ny) throw() :
        Y_PARA_SPLIT2D_INIT()
        {
            init(Nx,Ny);
        }

        split:: in2D:: in2D(const size_t nproc, const patch2D &p) throw() :
        Y_PARA_SPLIT2D_INIT()
        {
            init(p.width.x,p.width.y);
        }


        size_t split::in2D::get_xrank(const size_t rank) const throw()
        {
            assert(rank<size);
            return rank % xsize;
        }

        size_t split::in2D::get_yrank(const size_t rank) const throw()
        {
            assert(rank<size);
            return rank / xsize;
        }

        size_t split::in2D:: get_rank(const size_t rx, const size_t ry) const throw()
        {
            assert(rx<xsize);
            assert(ry<ysize);
            const size_t rank = ry * xsize + rx;
            assert( get_xrank(rank) == rx );
            assert( get_yrank(rank) == ry );
            return rank;
        }
        
        
    }
    
}

