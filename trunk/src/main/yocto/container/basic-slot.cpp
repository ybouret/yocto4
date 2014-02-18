#include "yocto/container/basic-slot.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    
#define YOCTO_BASIC_SLOT_CTOR(ARGS) \
slot_(ARGS),\
info_(0),   \
kill_(0)
    
#if defined(_MSC_VER)
	// no param for macro
#pragma warning ( disable : 4003 )
#endif
    //__________________________________________________________________________
    //
    // basic_slot
    //__________________________________________________________________________
    basic_slot:: basic_slot() throw() :
    YOCTO_BASIC_SLOT_CTOR( )
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
    uint32_t               basic_slot:: hash() const throw() { return hash32(slot_.data,slot_.size); }


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
    
    void basic_slot:: basic_swap_with( basic_slot &other ) throw()
    {
        slot_.swap_with(other.slot_);
        cswap(info_,other.info_);
        cswap(kill_,other.kill_);
    }
    
}
