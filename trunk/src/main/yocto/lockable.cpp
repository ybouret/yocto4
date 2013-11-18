#include "yocto/lockable.hpp"

namespace yocto
{
		
	lockable:: ~lockable() throw() {}
	
	lockable:: lockable() throw() {}

	
	scoped_lock:: scoped_lock( lockable &host ) throw() : host_( host ) { host_.lock(); }
	scoped_lock:: ~scoped_lock() throw() { host_.unlock(); }
}
