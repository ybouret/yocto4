#include "yocto/rx/lexical/engine.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace lexical
		{
			
			engine:: ~engine() throw() {}
			
			
			engine:: engine() :
			accepters_(),
			callbacks_(),
			source_( NULL ),
			stop_( false )
			{
				
			}
			
			void engine:: on( pattern *p, rule *r ) throw()
			{
				assert(p!=NULL);
				assert(r!=NULL);
				assert( accepters_.size == callbacks_.size );
				accepters_.push_back( p );
				callbacks_.push_back( r );
			}
						
			void engine:: reset( source &src ) throw()
			{
				for( pattern *p = accepters_.head; p; p = p->next ) p->sendto(src);
			}
			
		}
		
	}
	
}