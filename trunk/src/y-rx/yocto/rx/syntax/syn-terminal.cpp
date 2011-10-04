#include "yocto/rx/syntax/terminal.hpp"
#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			
			terminal:: terminal( const string &n  ) :
			rule( ID, n )
			{
			}
			
			terminal:: ~terminal() throw()
			{
				
			}
			
			terminal * terminal:: create( const string &n ) 
			{
				return new terminal(n);
			}
			
			syntax_result terminal:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				std::cerr << "?match TERM " << name << std::endl;
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
						std::cerr << "-match TERM EOF" << std::endl;
						return syntax_nothing; //!< end of source
					}
					
				}
				else 
				{
					if( lx->label == name )
					{
						//-- match !
						std::cerr << "+match TERM " << name << std::endl;
						lexeme::destroy( lx, src.char_pool );
						
						return syntax_success;
					}
					else 
					{
						std::cerr << "-match TERM " << name << " / " << lx->label << std::endl;
						lxr.unget( lx );
						return syntax_success;
					}
				}
				
				
			}
			
			
		}
		
	}
	
}
