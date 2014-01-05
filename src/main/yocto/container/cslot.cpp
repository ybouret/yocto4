#include "yocto/container/cslot.hpp"

#include "yocto/code/round.hpp"
#include "yocto/object.hpp"
#include "yocto/code/bswap.hpp"

#include <cstring>

namespace yocto
{
    //__________________________________________________________________________
    //
    // cslot
    //__________________________________________________________________________
    cslot:: ~cslot() throw()
    {
        release();
    }
    
    cslot:: cslot() throw() :
    data(0),
    size(0)
    {
    }
    
    cslot:: cslot(size_t n) :
    data(0),
    size(0)
    {
        if(n>0)
        {
            const size_t na = YOCTO_MEMALIGN(n);
            data          = object:: operator new(na);
            (size_t&)size = na;
        }
    }
    
    void cslot:: swap_with( cslot &other ) throw()
    {
        cswap(data,other.data);
        cswap_const(size,other.size);
    }
    
    void cslot:: prepare(size_t n)
    {
        if(n>size)
        {
            cslot     tmp(n);
            swap_with(tmp);
        }
        else
        {
            memset(data,0,size);
        }
    }
    
    void cslot:: release() throw()
    {
        if(data)
        {
            assert(size>0);
            object:: operator delete(data,(size_t&)size);
        }
#ifndef NDEBUG
        else { assert(0==size); }
#endif
        
    }
}

