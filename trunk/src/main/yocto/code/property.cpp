#include "yocto/code/property.hpp"

namespace yocto 
{
	
	ppty:: ~ppty() throw() {}
	
	ppty:: ppty( bool rw ) throw() : writable_( rw )
	{
	}
	
	ppty:: ppty( const ppty &other ) throw() : writable_( other.writable_ )
	{
	}
	
	
	bool ppty:: is_writable() const throw() { return writable_; }
	
	void ppty:: set_writable( bool rw ) throw()
	{
		writable_ = rw;
	}
	
	const char * ppty:: name() const throw() { return "ppty"; }
}

