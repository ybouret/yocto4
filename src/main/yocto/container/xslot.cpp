#include "yocto/container/xslot.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    bool xslot:: is_active() const throw()
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
    
    
    
    void xslot:: __free() throw()
    {
        if(is_active())
        {
            kill_(slot_.data);
            kill_ = 0;
            info_ = 0;
            copy_ = 0;
        }
        assert(0==kill_);
        assert(0==info_);
        assert(0==copy_);
    }
    
    
    
    void xslot:: free() throw() { __free(); }
    
    xslot:: ~xslot() throw()
    {
        __free();
    }
    
    
    xslot:: xslot() throw() :
    basic_slot(),
    copy_(0)
    {
    }
    
    xslot:: xslot(size_t n):
    basic_slot(n),
    copy_(0)
    {
    }
    
    xslot:: xslot( const xslot &other ) :
    basic_slot( other.size() ),
    copy_(0)
    {
        if( other.is_active() )
        {
            other.copy_( slot_.data, other.slot_.data );
        }
        
        info_ = other.info_;
        kill_ = other.kill_;
        copy_ = other.copy_;
    }
    
    void xslot:: swap_with( xslot &other ) throw()
    {
        basic_swap_with(other);
        cswap(copy_,other.copy_);
    }
    
    
    xslot & xslot:: operator=( const xslot &other )
    {
        xslot tmp(other);
        swap_with(tmp);
        return *this;
    }
    
    
}
