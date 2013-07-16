#include "yocto/container/vslot.hpp"
#include "yocto/code/round.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    
    
    bool vslot:: is_allocated() const throw()
    {
        if( size > 0 )
        {
            assert(data!=0);
            return true;
        }
        else
        {
            assert(0==data);
            return false;
        }
    }
    
    bool vslot:: is_active() const throw()
    {
        if(type!=0)
        {
            assert(is_allocated());
            assert(kill!=0);
            return true;
        }
        else
        {
            assert(0==kill);
            return false;
        }
    }
    
    void vslot:: allocate( size_t n )
    {
        assert(0==size);
        assert(0==data);
        assert(0==kill);
        assert(0==type);
        if(n>0)
        {
            data = object:: operator new(n);
            size = n;
        }
    }

    
    void vslot:: deallocate() throw()
    {
        assert(0==type);
        assert(0==kill);
        if(is_allocated())
        {
            object:: operator delete(data,size);
            data = 0;
            size = 0;
        }
    }
    
    void vslot:: free() throw()
    {
        if(is_active())
        {
            kill(data);
            kill = 0;
            type = 0;
        }
        assert(0==kill);
        assert(0==type);
    }
    
    void vslot:: release() throw()
    {
        free();
        deallocate();
    }
    
    void vslot:: prepare_for(size_t n)
    {
        free();
        if(n>size)
        {
            deallocate();
            allocate(n);
        }
        
    }
    
    vslot:: ~vslot() throw()
    {
        release();
    }
    
    vslot:: vslot() throw() :
    size(0),
    data(0),
    type(0),
    kill(0)
    {
    }
    
    const std::type_info * vslot:: info() const throw()
    {
        return type;
    }
    
    size_t vslot:: bytes() const throw() { return size; }
    
    
}
