#include "yocto/rx/lexical/engine.hpp"
#include "yocto/rx/source.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace lexical
		{
			
			bool engine:: run( source &src )
			{
				assert( accepters_.size == callbacks_.size );
				
				//--------------------------------------------------------------
				//
				// Loop over patterns
				//
				//--------------------------------------------------------------
				stop_ = false;
				while( ( false == stop_ ) && ( NULL != src.peek() ) )
				{
					//----------------------------------------------------------
					//
					// Find a matching pattern
					//
					//----------------------------------------------------------
					pattern *best_p = NULL;
					rule    *best_r = NULL;
					pattern *p      = accepters_.head;
					rule    *r      = callbacks_.head;
					
					for( ;p; p=p->next, r=r->next )
					{
						if( p->accept(src) )
						{
							//--------------------------------------------------
							//-- found a pattern !
							//--------------------------------------------------
							best_p = p;
							best_r = r;
							goto FOUND;
						}
					}
					
					//--- no matching pattern => this engine doesn't work
					return false;
					
				FOUND:
					assert(best_p != NULL );
					assert(best_r != NULL );
					
					//----------------------------------------------------------
					/// Restore source to test further patterns
					//----------------------------------------------------------
					src.uncpy( *best_p );
					for( p=p->next, r=r->next;p; p=p->next, r=r->next )
					{
						if( p->accept( src  ) )
						{
							//--------------------------------------------------
							// we have an other pretendant
							//--------------------------------------------------
							if( p->size <= best_p->size )
							{
								//----------------------------------------------
								//-- too late...
								//----------------------------------------------
								src.unget( *p );
								p->sendto(src);
							}
							else 
							{
								//----------------------------------------------
								//-- new winner !
								//----------------------------------------------
								best_p->sendto(src);
								best_p = p;
								best_r = r;
								src.uncpy( *best_p );
							}

						}
					}
					
					//----------------------------------------------------------
					/// Now we have the best pattern
					//----------------------------------------------------------
					assert( src.in_cache() >= best_p->size );

					//-- skip source if there is a recursive all
					src.skip( best_p->size );

					//-- apply the rule
					best_r->apply( *best_p );
					
					//-- release the memory
					best_p->sendto(src);
					
				}
				
				return true;
				
			}
			
		}
		
	}
	
}
