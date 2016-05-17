#include "split2d.hpp"
#include "yocto/code/utils.hpp"
#include "basic.hpp"
#include <cstdlib>

namespace yocto
{
    namespace parallel
    {
        split2D:: ~split2D() throw()
        {
        }





        split2D:: split2D(const size_t num_procs, const patch2D &p ) :
        patch2D(p),
        cores(0),
        x_cores(0),
        y_cores(0)
        {
            const size_t N_cpu    = max_of<size_t>(1,num_procs);
            size_t      &nc       = (size_t&)cores;
            size_t      &nx       = (size_t&)x_cores;
            size_t      &ny       = (size_t&)y_cores;

            const size_t Lx     = p.width.x; assert(Lx>0);
            const size_t Ly     = p.width.y; assert(Ly>0);
            const size_t Nx     = min_of(N_cpu,Lx);
            const size_t Ny     = min_of(N_cpu,Ly);
            const size_t Theta0 = Lx*Ly;
            size_t Lv=0,Nu=0;
            if(Lx>Ly)
            {
                //split along x
                Lv=Ly;
                Nu=Nx;
                nx=Nx;
                ny=1;
            }
            else
            {
                Lv=Lx;
                Nu=Ny;
                nx=1;
                ny=Ny;
            }

            nc = Nu;
            const size_t  twoLv    = Lv+Lv;
            size_t        ThetaDen = Nu+Nu;
            size_t        ThetaNum = (ThetaDen+1)*Theta0;
            //std::cerr << "Theta=" << ThetaNum << "/" << ThetaDen << "=" << double(ThetaNum)/ThetaDen << std::endl;

            for(size_t px=2;px<=Nx;++px)
            {
                for(size_t py=2;py<=Ny;++py)
                {
                    const size_t p = px*py;
                    if(p>N_cpu)
                    {
                        break;
                    }
                    const size_t num = Theta0*(twoLv+px*Ly+py*Lx);
                    const size_t den = twoLv*p;
                    //std::cerr << "\t" << px << "x" << py << "=>" << num << "/" << den  << "=" << double(num)/den << std::endl;
                    const size_t lhs = ThetaDen * num;
                    const size_t rhs = ThetaNum * den;
                    if(lhs<rhs || (lhs==rhs&&p>=nc) )
                    {
                        //std::cerr << "\t\taccept" << std::endl;
                        ThetaNum = num;
                        ThetaDen = den;
                        nx       = px;
                        ny       = py;
                        nc       = p;
                    }
                }
            }

            assert(cores>0);
            assert(cores<=num_procs);
            assert(x_cores*y_cores==cores);
        }

        patch2D split2D:: operator()(size_t rank) const throw()
        {
            assert(rank<cores);
            const ldiv_t d         = ldiv(rank,x_cores);
            const size_t y_rank    = d.quot;
            const size_t x_rank    = d.rem;
            coord2D      start     = lower;
            coord2D      final     = width;

            basic_split(x_rank,x_cores, start.x, final.x);
            basic_split(y_rank,y_cores, start.y, final.y);
            final += start;
            --final.x;
            --final.y;
            return patch2D(start,final);
        }
        
        
        
    }
}
