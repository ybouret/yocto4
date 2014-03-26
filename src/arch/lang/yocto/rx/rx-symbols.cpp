#include "yocto/rx/symbols.hpp"
#include "yocto/code/utils.hpp"
#include <iostream>

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
		
		
		void  first_chars:: merge( const symbols &other )
		{
			for( const_iterator i = other.begin(); i != other.end(); ++i )
			{
				(void) insert( *i );
			}
			
		}
		
		std::ostream & operator<<( std::ostream &os, const first_chars &fch )
		{
			std::cerr << '[';
			if( fch.accept_empty ) 
				std::cerr << '!';
			else 
				std::cerr << '+';
			std::cerr << ']';
			
			std::cerr << '<';
			for( symbols::const_iterator i = fch.begin(); i != fch.end(); ++i )
			{
				const char C = *i;
				if( C >= 32 && C < 127 )
				{
					std::cerr << C;
				}
				else
				{					
					std::cerr << "\\x";
					std::cerr << hexa_char[ ((*i) >> 4) & 0xf ];
					std::cerr << hexa_char[  (*i)       & 0xf ];
				}

			}
			std::cerr << '>';
			return os;
		}
		
	}
}
