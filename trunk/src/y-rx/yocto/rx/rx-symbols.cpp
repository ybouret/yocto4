#include "yocto/rx/symbols.hpp"

namespace yocto
{
	namespace regex 
	{
		first_chars::  first_chars() throw() : accept_empty(true) {}
		first_chars:: ~first_chars() throw() {}
		
		first_chars:: first_chars( const first_chars &other ) : 
		symbols( other ),
		accept_empty(other.accept_empty)
		{
		}
		

	}
}
