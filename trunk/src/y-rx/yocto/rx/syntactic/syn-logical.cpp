#include "yocto/rx/syntactic/logical.hpp"
#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			//==================================================================
			//
			//==================================================================
			logical:: ~logical() throw()
			{
			}
			
			logical:: logical( uint32_t t, const string &n  ):
			rule(t,n ),
			operands()
			{
			}
			
			logical & logical:: operator<<( rule *r ) throw()
			{
				assert( r != NULL );
				operands.push_back(r);
				r->parent = this;
				return *this;
			}
			
			logical:: logical( const logical &other ) :
			rule( other.type, other.name ),
			operands( other.operands )
			{
				assert( operands.size == other.operands.size );
				assert( other.type == type );
				assert( other.name == name );
				for( rule *r = operands.head; r; r=r->next )
				{
					r->parent = this;
				}
			}
			
		}
	}
	
}
