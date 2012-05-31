#include "yocto/lang/syntax/terminal.hpp"

namespace yocto 
{
	namespace lang
	{

		namespace syntax
		{

			terminal:: ~terminal() throw() {}

			terminal:: terminal( const string &id ) : rule( id ) {}

			bool  terminal:: match( Y_SYNTAX_MATCH_ARGS )
			{
				lexeme *lx = Lexer.next_lexeme( Source );
				if( !lx )
					return false; //EOF

				if( lx->label == this->label )
				{
					try 
					{ 
						parse_node *node = new parse_node(this->label,lx);

					}
					catch(...)
					{
						Lexer.unget(lx);
						throw;
					}
					return true;
				}
				else
					return false;
			}
		}

	}

}
