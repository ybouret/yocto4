#include "yocto/container/container.hpp"
#include "yocto/code/round.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>
//#include <iostream>

namespace yocto
{
	
	
	container::  container() throw() {}
	container:: ~container() throw() {}
	
	size_t container:: available() const throw()
	{
		assert( capacity() >= size() );
		return capacity() - size();
	}
	
	size_t container:: next_capacity( size_t n )
	{
		size_t m = n;
		++m;
		m += (m>>1);
		m = YOCTO_ROUND(YOCTO_CONTAINER_CAPACITY_LN2,m);
		if( m <= n )
			throw libc::exception( ERANGE, "container capacity overflow@%u", unsigned(n) );
		
		return m;
	}
	
	size_t container:: next_increase( size_t n )
	{
		return next_capacity(n) - n;
	}
	
	
	void container:: ensure( size_t n )
	{
        //std::cerr << "ensure " << n << "/capacity=" << capacity() << std::endl;
		if( capacity() < n )
        {
            //std::cerr << "\treserving " << n-capacity() << std::endl;
            reserve(n-capacity());
        }
	}
}
