#include "yocto/parallel/split.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace parallel
    {
        patch1D split::compute(size_t rank,size_t size,const patch1D &source)
        {
            unit_t offset = source.lower;
            unit_t length = source.width;
            split::in1D(rank, size, offset, length);
            if(length<=0) throw libc::exception( EDOM, "split::in1D produced negative length");
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
                double Theta = -1;
                double beta  = -1;
                // compute beta, the minimal local transfert time
                // to beat the sequential time
                if(Ly<Lx)
                {
                    // linear split along X
                    nx = size;
                    ny = 1;
                    const double twoLy = Ly+Ly;
                    beta  = twoLy / (Lx+twoLy);
                    //std::cerr << "\tbeta(x>y) = " << beta << std::endl;
                    Theta = twoLy +  ((beta+beta)*Lx)/double(size);
                }
                else
                {
                    // linear split along Y, default...
                    nx = 1;
                    ny = size;
                    const double twoLx = Lx+Lx;
                    beta  = twoLx / (Ly+twoLx);
                    //std::cerr << "\tbeta(y>=x)= " << beta << std::endl;
                    Theta = twoLx +  ((beta+beta) * Ly)/double(size);
                }
                //std::cerr << "\tTheta = " << Theta << std::endl;
                //std::cerr << "\tSeqTm = " << 2*beta*(Lx+Ly) << std::endl;

                for(size_t j=2;j<=size;++j)
                {
                    for(size_t i=2;i<=size;++i)
                    {
                        if(i*j==size)
                        {
                            const double tmp = double( ( (Lx*j) + (Ly*i) ) << 1 ) / size;
                            //std::cerr << "\t\ti=" << i << ", j=" << j << ", theta=" << tmp << std::endl;
                            if(tmp<=Theta)
                            {
                                Theta = tmp;
                                nx    = i;
                                ny    = j;
                            }
                        }
                    }
                }
            }
        }



        split:: in2D:: in2D(const size_t nproc, const patch2D &p) throw() :
        size( nproc <= 0 ? 1 : nproc ),
        xsize(0),
        ysize(0),
        offset(p.lower),
        length(p.width)
        {
            init(length.x,length.y);
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

        patch2D split::in2D:: operator()(const size_t rank) const
        {
            assert(rank<size);
            const size_t xrank = get_xrank(rank); assert(xrank<xsize);
            const size_t yrank = get_yrank(rank); assert(yrank<ysize);

            unit_t xoffset = offset.x;
            unit_t xlength = length.x;

            unit_t yoffset = offset.y;
            unit_t ylength = length.y;

            in1D(xrank, xsize,xoffset,xlength); if(xlength<=0) throw libc::exception( EDOM, "split::in2D produced negative xlength");
            in1D(yrank, ysize,yoffset,ylength); if(ylength<=0) throw libc::exception( EDOM, "split::in2D produced negative ylength");

            return patch2D( coord2D(xoffset,yoffset), coord2D(xoffset+xlength-1,yoffset+ylength-1));
        }


    }
    
}

