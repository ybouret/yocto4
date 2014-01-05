#include "yocto/container/vslot.hpp"
#include "yocto/code/round.hpp"
#include "yocto/object.hpp"
#include "yocto/code/bswap.hpp"

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
    
#define YOCTO_BASIC_SLOT_CTOR(ARGS) \
slot_(ARGS),\
info_(0),   \
kill_(0)
    
    //__________________________________________________________________________
    //
    // basic_slot
    //__________________________________________________________________________
    basic_slot:: basic_slot() throw() :
    YOCTO_BASIC_SLOT_CTOR()
    {
        
    }
    
    basic_slot:: basic_slot(size_t n) :
    YOCTO_BASIC_SLOT_CTOR(n)
    {
    }
    
    
    basic_slot:: ~basic_slot() throw()
    {
        assert(0==kill_);
    }
    
    size_t                 basic_slot:: size() const throw() { return slot_.size; }
    const std::type_info * basic_slot:: info() const throw() { return info_; }
    const char *           basic_slot:: name() const throw() { return info_ ? info_->name() : ""; }
    
    void basic_slot:: allocate(size_t n)
    {
        assert( !is_active() );
        slot_.prepare(n);
    }
    
    void basic_slot:: deallocate() throw()
    {
        assert( !is_active() );
        
    }
    
    void basic_slot:: release() throw()
    {
        free();
        deallocate();
    }
    
    bool basic_slot:: is_allocated() const throw() { return slot_.size > 0; }

    void basic_slot:: prepare_for(size_t n) { free(); slot_.prepare(n); }

    
    //__________________________________________________________________________
    //
    //
    //__________________________________________________________________________
    
    bool vslot:: is_active() const throw()
    {
        if(info_!=0)
        {
            assert(is_allocated());
            assert(kill_!=0);
            return true;
        }
        else
        {
            assert(0==kill_);
            return false;
        }
    }
    
    
    
    void vslot:: __free() throw()
    {
        if(is_active())
        {
            kill_(slot_.data);
            kill_ = 0;
            info_ = 0;
        }
        assert(0==kill_);
        assert(0==info_);
    }
    
    
    
    void vslot:: free() throw() { __free();    }
    
    vslot:: ~vslot() throw()
    {
        __free();
    }
    
    
    vslot:: vslot() throw() :
    basic_slot()
    {
    }
    
    vslot:: vslot(size_t n):
    basic_slot(n)
    {
    }
    
    
}
