#include "yocto/gfx/ops/differential.hpp"

namespace yocto
{
    namespace gfx
    {
        differential::  differential() :
        tgt(0),
        src(0),
        chn(0),
        coreProc(0),
        sideProcLo(0),
        sideProcUp(0),
        evalProc(0),
        global(0,0)
        {}
        
        differential:: ~differential() throw() {}


        real_t differential:: core_proc_g(real_t Um, real_t, real_t Up) throw()
        {
            return (Up-Um);
        }

        real_t differential:: side_proc_g_lo(real_t U0, real_t U1, real_t U2) throw()
        {
            return (4*U1)-U2-(3*U0);
        }

        real_t differential:: side_proc_g_up(real_t U0, real_t U1, real_t U2) throw()
        {
            return -side_proc_g_lo(U0,U1,U2);
        }

        real_t differential:: eval_proc_g(real_t gx, real_t gy) throw()
        {
            return math::Sqrt(gx*gx+gy*gy);
        }

        real_t differential:: core_proc_l(real_t Um, real_t U0, real_t Up) throw()
        {
            return (Up+Um)-(U0+U0);
        }

        real_t differential:: side_proc_l(real_t U0, real_t U1, real_t U2) throw()
        {
            return (U2+U0)-(U1+U1);
        }

        real_t differential:: eval_proc_l(real_t gx, real_t gy) throw()
        {
            return gx+gy;
        }

    }
}
