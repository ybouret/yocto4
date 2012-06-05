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
                    std::cerr << "/EOF" << std::endl;
					return false; //EOF
                }
                
                std::cerr << "<==== <" << lx->label << ">" << std::endl;
				if( lx->label == this->label )
				{
                    std::cerr << "+TERM <" << label << ">" << std::endl;
                    parse_node *Node = parse_node::create(this->label,lx);
					grow( Tree, Node );
					return true;
				}
				else
                {
                    std::cerr << "-TERM <" << label << ">" << std::endl;
                    Lexer.unget(lx);
					return false;
                }
			}
        }
        
	}
    
}
