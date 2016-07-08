#include "yocto/graphics/ops/differential.hpp"


namespace yocto
{
    namespace graphics
    {
        differential:: ~differential() throw()
        {
        }

        differential:: differential(const unit_t W, const unit_t H) :
        pixmap<real_t>(W,H)
        {
        }


        real_t differential:: G_core(const real_t Um, const real_t, const real_t Up) throw()
        {
            return (Up-Um);
        }

        real_t differential:: G_side(const real_t U0, const real_t U1, const real_t U2) throw()
        {
            //return 0;
            return 4.0f * U1 - 3.0f * U0 - U2;
        }

        real_t differential:: G_func(const real_t dx, const real_t dy) throw()
        {
            return sqrt(dx*dx+dy*dy);
        }

        real_t differential:: L_core(const real_t Um, const real_t U0, const real_t Up) throw()
        {
            return (Up-(U0+U0)+Um);
        }

        real_t differential:: L_side(const real_t U0, const real_t U1, const real_t U2) throw()
        {
            return U2+U0 - (U1+U1);
        }

        real_t differential:: L_func(const real_t dx, const real_t dy) throw()
        {
            return dx+dy;
        }


    }
}
