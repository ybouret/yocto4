#include "yocto/rx/syntactic/terminal.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{

			terminal:: ~terminal() throw() {}
					
			terminal:: terminal( rule *p, const string &n ) : 
			rule( p, ID, n )
			{
			}
			
			syntax_result terminal:: match( lexer &lxr, source &src)
			{
				lexeme *lx = lxr.lookahead( src );
				if( !lx )
					return src.is_active() ? syntax_error : syntax_error;
				else
				{
					if( lx->label == name )
					{
						std::cerr << "match '" << name << "'=<" << (*lx) << ">" << std::endl;
						return syntax_success;
					}
					else 
					{
						return syntax_invalid;
					}
				}
			}

			
			
		}

	}

}
