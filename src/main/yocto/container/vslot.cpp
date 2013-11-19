#include "yocto/container/vslot.hpp"
#include "yocto/code/round.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    
    
    bool vslot:: is_allocated() const throw()
    {
        if( size_ > 0 )
        {
            assert(data_!=0);
            return true;
        }
        else
        {
            assert(0==data_);
            return false;
        }
    }
    
    bool vslot:: is_active() const throw()
    {
        if(type_!=0)
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
    
    void vslot:: allocate( size_t n )
    {
        assert(0==size_);
        assert(0==data_);
        assert(0==kill_);
        assert(0==type_);
        if(n>0)
        {
            const size_t na = YOCTO_MEMALIGN(n);
            data_ = object:: operator new(na);
            size_ = na;
        }
    }

    
    void vslot:: deallocate() throw()
    {
        assert(0==type_);
        assert(0==kill_);
        if(is_allocated())
        {
            object:: operator delete(data_,size_);
            data_ = 0;
            size_ = 0;
        }
    }
    
    void vslot:: free() throw()
    {
        if(is_active())
        {
            kill_(data_);
            kill_ = 0;
            type_ = 0;
        }
        assert(0==kill_);
        assert(0==type_);
    }
    
    void vslot:: release() throw()
    {
        free();
        deallocate();
    }
    
    void vslot:: prepare_for(size_t n)
    {
        free();
        if(n>size_)
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
    size_(0),
    data_(0),
    type_(0),
    kill_(0)
    {
    }
    
    vslot:: vslot(size_t n):
    size_(0),
    data_(0),
    type_(0),
    kill_(0)
    {
        allocate(n);
    }
    
    const std::type_info * vslot:: info() const throw()
    {
        return type_;
    }
    
    size_t vslot:: bytes() const throw() { return size_; }
    
    
}
