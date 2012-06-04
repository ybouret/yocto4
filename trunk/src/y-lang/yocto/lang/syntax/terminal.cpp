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
                std::cerr << "?TERM <" << label << ">" << std::endl;
				lexeme *lx = Lexer.next_lexeme( Source );
				if( !lx )
                {
					return false; //EOF
                }
                
				if( lx->label == this->label )
				{
					grow( Tree, parse_node::create(this->label,lx) );
					return true;
				}
				else
					return false;
			}
            
            terminal:: terminal( const terminal &other ) :
            rule( other )
            {
            }
            
            
            rule * terminal:: clone() const
            {
                return new terminal( *this );
            }
		}

	}

}
