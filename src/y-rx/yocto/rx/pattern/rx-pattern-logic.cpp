#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace logical
		{
			
			////////////////////////////////////////////////////////////////////
			Operator:: ~Operator() throw()
			{
			}
			
			Operator:: Operator( uint32_t t ) throw() : pattern( t ), operands()
			{
			}
			
			Operator:: Operator( const Operator &op ) : pattern( op.type ), operands( op.operands )
			{
				
			}
			
			void Operator:: sendto( source &src ) throw()
			{
				back_to(src.pool);
				for( pattern *p = operands.head; p; p=p->next)
				{
					p->back_to( src.pool );
				}
			}
			
			////////////////////////////////////////////////////////////////////
			AND:: AND() throw() : Operator( id )
			{
			}
			
			AND:: AND( const AND &op ) : Operator( op )
			{
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			bool AND:: accept( source & src )
			{
				assert( 0 == size );
				for( pattern *p = operands.head; p; p=p->next )
				{
					assert( 0 == p->size );
					if( p->accept( src ) )
					{
						p->at_tail( *this );
					}
					else 
					{
						src.unget( *this );
						assert(0==size);
						return false;
					}

				}
				return true;
			}
			
			
			
		}
		
	}
}
