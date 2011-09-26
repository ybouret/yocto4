#include "yocto/rx/syntactic/joker.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace syntactic
		{
		

			joker:: ~joker() throw() { assert(jk); delete jk; }
			joker::  joker( uint32_t t, const string &n, rule *r ) : 
			rule( t, n ),
			jk( r )
			{
			}
			
		}

	}
}
