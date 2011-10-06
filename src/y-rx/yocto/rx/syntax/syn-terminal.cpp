#include "yocto/rx/syntax/terminal.hpp"
#include <iostream>
#include "yocto/rx/grammar.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			
			////////////////////////////////////////////////////////////////////
			terminal:: terminal( const string &n, int p, grammar &g ) :
			rule( ID, n, p, g )
			{
			}
			
			terminal:: ~terminal() throw()
			{
			}
			
		
			
			syntax_result terminal:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				std::cerr << "?TERM " << name << std::endl;
				lexeme *lx = lxr.lookahead(src);
				if( !lx )
				{
					//----------------------------------------------------------
					//-- no lexeme
					//----------------------------------------------------------
					if( src.is_active() )
					{
						//-- no match at all: syntax error
						throw exception("syntax error", "looking for '%s': invalid '%c'", name.c_str(), src.peek()->data );
					}
					else 
					{
						//-- special case
						std::cerr << "-TERM (EOF)" << std::endl;
						return syntax_nothing; //!< end of source
					}
					
				}
				else 
				{
					if( lx->label == name )
					{
						//-- match !
						std::cerr << "+TERM " << name << std::endl;
						c_node::append(tree,c_node::create( lx, *this ));
						return syntax_success;
					}
					else 
					{
						std::cerr << "-TERM " << name << " ( got " << lx->label << " )" << std::endl;
						lxr.unget( lx );
						return syntax_unexpected;
					}
				}
				
				
			}
			
#if 0
			////////////////////////////////////////////////////////////////////
			variant:: variant( const string &n, grammar &g ) : terminal( ID,n,g ) {}
			variant:: ~variant() throw() {}
			
			////////////////////////////////////////////////////////////////////
			certain:: certain( const string &n, grammar &g ) : terminal( ID,n,g ) {}
			certain:: ~certain() throw() {}
			
			
			////////////////////////////////////////////////////////////////////
			useless:: useless( const string &n, grammar &g ) : terminal( ID,n,g ) {}
			useless:: ~useless() throw() {}
#endif
			
			
		}
		
	}
	
}
