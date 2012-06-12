#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	namespace lang
	{
        
		namespace syntax
		{
            
			terminal:: ~terminal() throw() {}
            
			terminal:: terminal( const string &id, node_property ppty, const string &eof_ref ) : 
            rule( id ),
            semantic( ppty ),
            eof(eof_ref)
            {
                switch( semantic )
                {
                    case is_merging_all:
                        throw exception("terminal<%s>: invalid property", id.c_str());
                    default:
                        break;
                }
            }
            
			bool  terminal:: match( Y_SYNTAX_MATCH_ARGS )
			{
                //std::cerr << "?TERM <" << label << ">" << std::endl;
                check(Tree);
                
				lexeme *lx = Lexer.next_lexeme( Source );
				if( !lx )
                {
                    std::cerr << "</EOF>" << std::endl;
                    lx = new lexeme(eof,Lexer.line);
                    Lexer.unget(lx);
					return false; //EOF
                }
                
                if( lx->label == eof )
                {
                    Lexer.unget(lx);
                    return false;
                }
                
                //std::cerr << "<==== <" << lx->label << ">" << std::endl;
                //Context.lx = lx;      //!< record the last accepted term
				if( lx->label == this->label )
				{
                    //std::cerr << "+TERM <" << label << ">" << std::endl;
                    parse_node *Node = parse_node::create(this->label,lx,semantic);
                    grow( Tree, Node );
					return true;
				}
				else
                {
                    //std::cerr << "-TERM <" << label << ">" << std::endl;
                    Lexer.unget(lx);
					return false;
                }
			}
        }
        
	}
    
}
