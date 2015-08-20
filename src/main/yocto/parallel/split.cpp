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


namespace yocto
{
    namespace parallel
    {
        split::in2D:: ~in2D() throw() {}

#if 0
        static inline
        double computeTime(const size_t i, const size_t Nx,
                           const size_t j, const size_t Ny)
        {
            assert(i>0);
            if(i==1)
            {

            }
            else
            {

            }
        }
#endif

        void split:: in2D:: init(const size_t Nx, const size_t Ny) throw()
        {
            assert(Nx>0);
            assert(Ny>0);
            size_t &nx  = (size_t &)xsize;
            size_t &ny  = (size_t &)ysize;
            double  tau = -1;
            for(size_t i=1;i<=size;++i)
            {
                for(size_t j=1;j<=size;++j)
                {
                    if(i*j==size)
                    {
                        const double tmp = double(Nx)/i + double(Ny)/j;
                        if(tau<0 || tmp<tau)
                        {
                            nx  = i;
                            ny  = j;
                            tau = tmp;
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

