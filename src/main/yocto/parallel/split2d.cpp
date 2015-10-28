#include "split2d.hpp"
#include "yocto/code/utils.hpp"

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
            const size_t max_cpus = max_of<size_t>(1,num_procs);
            size_t      &cpus     = (size_t&)cores;
            size_t      &nx       = (size_t&)x_cores;
            size_t      &ny       = (size_t&)y_cores;

            const size_t Lx     = p.width.x; assert(Lx>0);
            const size_t Ly     = p.width.y; assert(Ly>0);
            const size_t nxMax  = min_of(max_cpus,Lx);
            const size_t nyMax  = min_of(max_cpus,Ly);
            const double Theta0   = Lx*Ly; // in time of item
            double       ThetaMin = -1;
            double       alpha    = 0;
            double       alpha_den = 0;
            // initialize
            if(Lx>Ly)
            {
                // cut along x
                nx    = nxMax;
                ny    = 1;
                alpha_den = 4*Ly;
                alpha = Theta0/(4*Ly);
                ThetaMin = (Theta0+Theta0+nx*Theta0)/(nx+nx);
                //std::cerr << "Initial cut long x" << std::endl;
            }
            else
            {
                // Lx<=Ly
                nx       = 1;
                ny       = nyMax;
                alpha    = Theta0/(4*Lx);
                ThetaMin = (Theta0+Theta0+ny*Theta0)/(ny+ny);
                //std::cerr << "Initial cut along y" << std::endl;
            }
            cpus = nx*ny;
            //std::cerr << "alpha   =" << alpha    << std::endl;
            //std::cerr << "ThetaMin=" << ThetaMin << std::endl;
            //std::cerr << "nx=" << nx <<", ny=" << ny << ", cpus=" << cpus << std::endl;
            
            for(size_t px=2;px<=nxMax;++px)
            {
                for(size_t py=2;py<=nyMax;++px)
                {
                    const size_t n = px*py;
                    if(n>max_cpus)
                    {
                        break;
                    }

                    const double Theta=Theta0*(alpha_den+2*(py*Lx+px*Ly))/(n*alpha_den);

                    //std::cerr << "px=" << px << ", py=" << py << ", n=" << n << ", Theta=" << Theta << std::endl;
                    if(Theta<=ThetaMin)
                    {
                        nx  = px;
                        ny  = py;
                        cpus = n;
                    }
                }
            }


        }




    }
}
