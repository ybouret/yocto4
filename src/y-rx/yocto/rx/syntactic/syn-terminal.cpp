#include "yocto/rx/syntactic/terminal.hpp"

#include <iostream>

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntactic
		{

			terminal:: ~terminal() throw() {}
					
			terminal:: terminal( const string &n ) : 
			rule( ID, n )
			{
			}
			
			syntax::result terminal:: analyze( lexer &lxr, source &src, lexemes &stk)
			{
				lexeme *lx = lxr.lookahead( src );
				if( !lx )
					return src.is_active() ? syntax::error : syntax::nothing;
				else
				{
					if( lx->label == name )
					{
						std::cerr << "match '" << name << "'=<" << (*lx) << ">" << std::endl;
						stk.push_back( lx );
						return syntax::success;
					}
					else 
					{
						lxr.unget( lx );
						return syntax::unexpected;
					}
				}
			}

			
			
		}

	}

}
