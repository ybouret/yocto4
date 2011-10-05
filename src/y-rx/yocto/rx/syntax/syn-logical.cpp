#include "yocto/rx/syntax/logical.hpp"
#include "yocto/rx/grammar.hpp"

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
			
			logical:: logical( uint32_t t, const string &n, grammar &g ) :
			rule(t,n,g),
			operands(2,as_capacity)	
			{
			}
			
			logical & logical:: operator<<( const string &p )
			{
				operands.push_back( G[p] );
				return *this;
			}
			
			logical & logical:: operator<<( const char *p )
			{
				operands.push_back( G[p] );
				return *this;
			}
			
			
			////////////////////////////////////////////////////////////////////
			aggregate:: aggregate( const string &n, grammar &g ) :
			logical(ID,n,g)
			{
				
			}
			
			aggregate:: ~aggregate() throw()
			{
			}
			
			
			
			syntax_result aggregate:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				//lexemes stk;
				std::cerr << "?match AGGR " << name << " [";
				for( size_t i=1; i <= operands.size(); ++i )
				{
					std::cerr << " " << operands[i]->name;
				}
				std::cerr << " ]" << std::endl;
				for( size_t i=1; i <= operands.size(); ++i )
				{
					const syntax_result res = operands[i]->match( lxr, src );
					if( res != syntax_success )
					{
						std::cerr << "-match AGGR " << name  << std::endl;
						return res;
					}
				}
				std::cerr << "+match AGGR " << name  << std::endl;
				return syntax_success;
			}
			
			////////////////////////////////////////////////////////////////////
			alternative:: alternative( const string &n, grammar &g ) :
			logical(ID,n,g)
			{
				
			}
			
			alternative:: ~alternative() throw()
			{
			}
			
			
			
			syntax_result alternative:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				//lexemes stk;
				std::cerr << "?match ALTR " << name << " [";
				for( size_t i=1; i <= operands.size(); ++i )
				{
					std::cerr << " " << operands[i]->name;
				}
				std::cerr << " ]" << std::endl;
				syntax_result res = syntax_success;
				for( size_t i=1; i <= operands.size(); ++i )
				{
					res = operands[i]->match( lxr, src );
					if( res == syntax_success )
					{
						std::cerr << "+match ALTR " << name  << std::endl;
						return syntax_success;
					}
				}
				std::cerr << "-match ALTR " << name  << std::endl;
				return res;
			}
			
			
		}
		
	}
	
}
