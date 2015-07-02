#include "yocto/threading/context.hpp"


namespace yocto
{
    namespace threading
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // a context: rank/size/access
        //
        ////////////////////////////////////////////////////////////////////////
        context:: ~context() throw() {}
        
        context::  context( size_t r, size_t s, lockable &lock_ref) throw() :
        vslot(),
        rank(r),
        indx(rank+1),
        size(s),
        access(lock_ref)        {
        }
        
    }
    
}


namespace yocto
{
    namespace threading
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // a self-contained, not threaded context
        //
        ////////////////////////////////////////////////////////////////////////
        single_context:: ~single_context() throw() {}
        
#if defined(_MSC_VER)
		// 'this' in constructor
#pragma warning ( disable : 4355 )
#endif
        single_context:: single_context() throw() :
        faked_lock(),
        context(0,1,*this)
        {
        }
        
        
        
        
        
    }
}


