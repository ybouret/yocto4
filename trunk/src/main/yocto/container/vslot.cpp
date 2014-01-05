#include "yocto/container/vslot.hpp"

namespace yocto
{
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
    
    void vslot:: swap_with(vslot &other ) throw()
    {
        basic_swap_with(other);
    }

    
    
}
