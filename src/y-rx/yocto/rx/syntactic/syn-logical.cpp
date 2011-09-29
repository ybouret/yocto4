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
			
			
			AND:: AND( const string &n ) :
			logical( ID, n )
			{
			}
			
			AND * AND:: create( const string &id )
			{
				return new AND(id);
			}
			
			AND * AND:: create( const char *txt )
			{
				const string id( txt );
				return new AND(id);
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			syntax::result AND:: match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				if( operands.size <= 0 )
					return syntax::success;
				else 
				{
					
					rule   *r = operands.head; assert( r != NULL );
					//----------------------------------------------------------
					// first rule
					//----------------------------------------------------------
					{
						//const syntax::result res = r->match( lxr, src, local_stk );
						
					}
					return syntax::nothing;
				}
			}
		}
	}
	
}
