#include "yocto/rx/syntax/logical.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			////////////////////////////////////////////////////////////////////
			logical:: ~logical() throw()
			{
			}
			
			logical:: logical( uint32_t t, const string &n ) :
			rule(t,n),
			operands(2,as_capacity)
			{
			}
			
			////////////////////////////////////////////////////////////////////
			aggregate:: aggregate( const string &n ) :
			logical(ID,n)
			{
				
			}
			
			aggregate:: ~aggregate() throw()
			{
			}
			
			aggregate * aggregate::create( const string &n ) { return new aggregate(n); }

		
			syntax_result aggregate:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				//lexemes stk;
				std::cerr << "?math AGGR " << name  << std::endl;
				for( size_t i=1; i <= operands.size(); ++i )
				{
					const syntax_result res = operands[i]->match( lxr, src );
					if( res != syntax_success )
					{
						return res;
					}
				}
				std::cerr << "+math AGGR " << name  << std::endl;
				return syntax_success;
			}
			
			
		}

	}

}
