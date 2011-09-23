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
			
			logical:: logical( rule *p, uint32_t t, const string &n ) :
			rule(p,t,n),
			operands()
			{
			}
			
			
			AND:: AND( rule *p, const string &n ) :
			logical( p, ID, n )
			{
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			
		}
	}

}
