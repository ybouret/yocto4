#include "yocto/code/property.hpp"
#include "yocto/exception.hpp"

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

    void ppty:: not_writable(const char *name) const
    {
        if(name)
        {
            throw exception("[property] '%s' is read-only", name);
        }
        else
        {
            throw exception("[property] is read-only");
        }
    }

}

