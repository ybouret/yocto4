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

            cpus = 1;
            nx   = ny = 1;
            const size_t Lx     = p.width.x;
            const size_t Ly     = p.width.y;
            const double Theta0 = Lx*Ly;



        }




    }
}
