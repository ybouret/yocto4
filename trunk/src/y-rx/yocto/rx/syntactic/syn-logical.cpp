#include "yocto/rx/syntactic/logical.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{
			logical:: ~logical() throw()
			{
			}
			
			logical:: logical( uint32_t t, const string &n ) :
			rule(t,n),
			operands()
			{
			}
			
			
			AND:: AND( const string &n ) :
			logical( ID, n )
			{
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			
		}
	}

}
