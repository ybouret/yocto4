#include "yocto/threading/context.hpp"


namespace yocto
{
    namespace threading
    {
        
        context:: ~context() throw() {}
        
        context::  context( size_t r, size_t s, lockable &lock_ref) throw() :
        rank(r),
        indx(rank+1),
        size(s),
        access(lock_ref)
        {
        }
     
        
        single_context:: ~single_context() throw() {}
        
        single_context:: single_context() throw() :
        faked_lock(),
        context(0,1,*this)
        {
        }
        
    }
}
