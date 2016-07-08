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
            return 0;
        }
    }
}
