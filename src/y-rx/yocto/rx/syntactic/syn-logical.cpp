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
			
			syntax::result AND:: analyze( lexer &lxr, source &src, lexemes &stk, context &ctx)
			{
				lexemes local_stk;
				for( rule *r = operands.head; r; r=r->next )
				{
					lexemes stk1;
					const syntax::result res = r->match( lxr, src, stk1, ctx, int(syntax::unexpected) );
					if( res != syntax::success )
					{
						lxr.unget( local_stk );
						return res;
					}
					local_stk.merge_back( stk1 );
				}
				
				std::cerr << "match '" << name << "'" << std::endl;
				stk.merge_back(local_stk);
				return syntax::success;
			}
		}
	}

}
