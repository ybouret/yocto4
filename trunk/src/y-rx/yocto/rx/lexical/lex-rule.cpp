#include "yocto/rx/lexical/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace lexical 
		{
			
			rule:: ~rule() throw() 
			{
				assert( NULL == next );
				assert( NULL == prev );
			}
			
			rule:: rule() throw() : next( NULL ), prev( NULL ) {}
			
			
			
			////////////////////////////////////////////////////////////////////////
			make:: make( const action &a ) : action_( a ) {}
			make:: ~make() throw() {}
			void make:: apply( const token &tkn ) 
			{
				action_( tkn );
			}
			
			
		}
	}
	
}