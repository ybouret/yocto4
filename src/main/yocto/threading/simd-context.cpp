#include "yocto/threading/simd.hpp"


namespace yocto
{
    namespace threading
    {

        SIMD::Context:: Context(size_t    r,
                                size_t    s,
                                lockable &lock_ref) throw() :
        //vslot(),
        rank(r),
        indx(rank+1),
        size(s),
        access(lock_ref)
        {
        }

        SIMD:: Context:: ~Context() throw()
        {

        }


        SIMD::SingleContext::SingleContext() throw() :
        Context(0,1,*this)
        {
        }

        SIMD:: SingleContext:: ~SingleContext() throw()
        {
        }
    }
}

