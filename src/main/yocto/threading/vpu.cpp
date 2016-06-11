#include "yocto/threading/vpu.hpp"

namespace yocto
{
    namespace threading
    {

        vpu:: ~vpu() throw()
        {
        }

        vpu:: vpu( const kexec_ptr &kxp ) throw() :
        cores(kxp->num_threads()),
        simd(kxp),
        call(this, & vpu::__call )
        {
        }


        void vpu:: __call(context &ctx) throw()
        {
        }
        
    }
}
