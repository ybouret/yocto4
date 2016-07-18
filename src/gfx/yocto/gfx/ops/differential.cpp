#include "yocto/gfx/ops/differential.hpp"

namespace yocto
{
    namespace gfx
    {
        differential::  differential() :
        tgt(0), src(0), chn(0),
        coreProc(0), sideProc(0), evalProc(0)
        {}
        
        differential:: ~differential() throw() {}


        real_t differential:: core_proc_g(real_t Um, real_t, real_t Up) throw()
        {
            return (Up-Um);
        }

        real_t differential:: side_proc_g(real_t U0, real_t U1, real_t U2) throw()
        {
            return (4*U1)-U2-(3*U0);
        }

        real_t differential:: eval_proc_g(real_t gx, real_t gy) throw()
        {
            return math::Sqrt(gx*gx+gy*gy);
        }
    }
}
