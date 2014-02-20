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
        rank(r),
        indx(rank+1),
        size(s),
        access(lock_ref)
        {
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
        
        single_context:: single_context() throw() :
        faked_lock(),
        context(0,1,*this)
        {
        }
        
        
        
        
        
    }
}

#include "yocto/memory/global.hpp"
namespace yocto
{
    namespace threading
    {
        context_batch:: ~context_batch() throw()
        {
            size_t i=size;
            while(i>0)
            {
                ctx[--i].~context();
            }
            memory::kind<memory::global>::release_as<context>(ctx, cnt);
        }
        
        context_batch:: context_batch( size_t n, lockable &lock_ref) :
        size( n > 0 ? n : 1),
        cnt(size),
        ctx( memory::kind<memory::global>::acquire_as<context>(cnt) )
        {
            
        }
    }
}

