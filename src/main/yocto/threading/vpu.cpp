#include "yocto/threading/vpu.hpp"

namespace yocto
{
    namespace threading
    {

        VPU:: ~VPU() throw()
        {
        }

        VPU:: VPU( const kexec_ptr &kxp ) throw() :
        SIMD(kxp),
        CALL(this, & VPU::call )
        {
        }


        void VPU:: call(context &ctx) throw()
        {
        }
        
    }
}
