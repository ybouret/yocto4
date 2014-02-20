#include "yocto/lockable.hpp"

namespace yocto
{
		
	lockable:: ~lockable() throw() {}
	
	lockable:: lockable() throw() {}

    
    faked_lock:: faked_lock() throw() : depth(0)
    {
        
    }
	
    void faked_lock:: lock() throw()
    {
        assert(depth>=0);
        ++depth;
    }
    
    void faked_lock:: unlock() throw()
    {
        --depth;
        assert(depth>=0);
    }
    
    bool faked_lock:: try_lock() throw()
    {
        ++depth;
        return true;
    }
    
    faked_lock:: ~faked_lock() throw()
    {
        assert(0==depth);
    }
    
    
    
	scoped_lock:: scoped_lock( lockable &host ) throw() : host_( host ) { host_.lock(); }
	scoped_lock:: ~scoped_lock() throw() { host_.unlock(); }
}
