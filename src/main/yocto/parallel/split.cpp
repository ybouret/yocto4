#include "yocto/parallel/split.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/parallel/basic.hpp"

namespace yocto
{
    namespace parallel
    {
        split:: in1D:: in1D(const size_t nproc, const patch1D &p ) throw() :
        cores(clamp<size_t>(1,nproc,p.width)),
        offset(p.lower),
        length(p.width)
        {
        }

        split:: in1D:: ~in1D() throw()
        {}


        patch1D split::in1D:: operator()(const size_t rank) const throw()
        {
            unit_t xoff = offset;
            unit_t xlen = length;
            assert(unit_t(cores)<=xlen);
            basic_split(rank, cores, xoff, xlen);assert(xlen>0);
            return patch1D(xoff,xoff+xlen-1);
        }




    }

}

#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace parallel
    {

        split::in2D:: ~in2D() throw() {}


        double split:: in2D:: init(const size_t Lx, const size_t Ly) throw()
        {
            assert(Lx>0);
            assert(Ly>0);
            size_t &N  = (size_t &)cores;
            size_t &nx = (size_t &)xsize;
            size_t &ny = (size_t &)ysize;
            if(cores<=1)
            {
                assert(1==cores);
                nx = ny =1;
                return -1;
            }
            else
            {
                double Theta = -1;
                double beta  = -1;
                const size_t nx_max = min_of(cores,Lx);
                const size_t ny_max = min_of(cores,Ly);
                // compute beta, the minimal local transfert time
                // to beat the sequential time
                if(Ly<Lx)
                {
                    // linear split along X
                    nx = nx_max;
                    ny = 1;
                    const size_t ntmp  = nx*ny;
                    const double twoLy = Ly+Ly;
                    beta  = twoLy / (Lx+twoLy);
                    //std::cerr << "\tbeta(x>y) = " << beta << std::endl;
                    Theta = twoLy +  ((beta+beta)*Lx)/double(ntmp);
                }
                else
                {
                    // linear split along Y, default...
                    nx = 1;
                    ny = ny_max;
                    const size_t ntmp  = nx*ny;
                    const double twoLx = Lx+Lx;
                    beta  = twoLx / (Ly+twoLx);
                    //std::cerr << "\tbeta(y>=x)= " << beta << std::endl;
                    Theta = twoLx +  ((beta+beta) * Ly)/double(ntmp);
                }
                //std::cerr << "\tTheta = " << Theta << std::endl;
                //std::cerr << "\tSeqTm = " << 2*beta*(Lx+Ly) << std::endl;

                for(size_t j=2;j<=ny_max;++j)
                {
                    for(size_t i=2;i<=nx_max;++i)
                    {
                        if(i>Lx)
                            break;

                        if(i*j==cores)
                        {
                            const double tmp = double( ( (Lx*j) + (Ly*i) ) << 1 ) / cores;
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
                N = nx*ny;
                return Theta;
            }
        }



        split:: in2D:: in2D(const size_t nproc, const patch2D &p) throw() :
        cores( nproc <= 0 ? 1 : nproc ),
        xsize(0),
        ysize(0),
        offset(p.lower),
        length(p.width),
        timing(init(length.x,length.y))
        {

        }


        size_t split::in2D::get_xrank(const size_t rank) const throw()
        {
            assert(rank<cores);
            return rank % xsize;
        }

        size_t split::in2D::get_yrank(const size_t rank) const throw()
        {
            assert(rank<cores);
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

        patch2D split::in2D:: operator()(const size_t rank) const throw()
        {
            assert(rank<cores);
            const size_t xrank = get_xrank(rank); assert(xrank<xsize);
            const size_t yrank = get_yrank(rank); assert(yrank<ysize);

            unit_t xoffset = offset.x;
            unit_t xlength = length.x;

            unit_t yoffset = offset.y;
            unit_t ylength = length.y;

            basic_split(xrank, xsize,xoffset,xlength); assert(xlength>0);
            basic_split(yrank, ysize,yoffset,ylength); assert(ylength>0);

            return patch2D( coord2D(xoffset,yoffset), coord2D(xoffset+xlength-1,yoffset+ylength-1));
        }


    }
    
}

