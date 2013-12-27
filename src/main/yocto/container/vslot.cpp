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
            assert(copy_!=0);
            return true;
        }
        else
        {
            assert(0==kill_);
            assert(0==copy_);
            return false;
        }
    }
    
    void vslot:: allocate( size_t n )
    {
        assert(0==size_);
        assert(0==data_);
        assert(0==kill_);
        assert(0==copy_);
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
        assert(0==copy_);
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
            copy_ = 0;
            type_ = 0;
        }
        assert(0==kill_);
        assert(0==type_);
        assert(0==copy_);
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
    
#define VSLOT_CTOR() \
size_(0), \
data_(0), \
type_(0), \
kill_(0), \
copy_(0)
    
    vslot:: vslot() throw() :
    VSLOT_CTOR()
    {
    }
    
    vslot:: vslot(size_t n):
    VSLOT_CTOR()
    {
        allocate(n);
    }
    
    const std::type_info * vslot:: info() const throw()
    {
        return type_;
    }
    
    size_t vslot:: bytes() const throw() { return size_; }
    
    const char * vslot:: name() const throw()
    {
        return type_ ?  type_->name() : "";
    }
    
    vslot:: vslot( const vslot &other) :
    VSLOT_CTOR()
    {
        allocate(other.size_);
        if(other.copy_)
        {
            assert(other.type_);
            assert(other.kill_);
            assert(other.data_);
            assert(other.size_>0);
            assert(size_>0);
            try
            {
                other.copy_(data_,other.data_);
            }
            catch(...)
            {
                deallocate();
                throw;
            }
            type_ = other.type_;
            kill_ = other.kill_;
            copy_ = other.copy_;
        }
    }
    
}
