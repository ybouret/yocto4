#include "yocto/threading/vpu.hpp"

namespace yocto
{
    namespace threading
    {

        vpu:: ~vpu() throw()
        {
        }


        vpu:: vpu(kernel_executor *kxp) throw() :
        simd(kxp),
        cores(simd->num_threads())
        {
        }


    }
}
