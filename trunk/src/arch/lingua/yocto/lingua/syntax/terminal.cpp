#include "yocto/lingua/syntax/terminal.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace syntax
        {
            
            terminal:: ~terminal() throw() {}
            
            terminal:: terminal( const string &id, node_property ppty, const string &eof_ref ) :
            rule( id ),
            semantic( ppty ),
            eof(eof_ref)
            {
#if 0
                switch( semantic )
                {
                    case is_merging_all:
                        throw exception("terminal<%s>: invalid property", id.c_str());
                    default:
                        break;
                }
#endif
            }
            
            Y_SYNTAX_MATCH_TYPE terminal:: match( Y_SYNTAX_MATCH_ARGS )
            {
                check(Tree);
                //std::cerr << "?TERM <" << label << ">" << std::endl;
                
                lexeme *lx = Lexer.get( Source );
                if( !lx )
                {
                    //std::cerr << "</EOF>" << std::endl;
                    lx = new lexeme(eof,Lexer.line);
                    Lexer.unget(lx);
                    return 0;
                }
                else
                {
                    
                    if( lx->label == eof )
                    {
                        Lexer.unget(lx);
                        return 0;
                    }
                    
                    //std::cerr << "<==== <" << lx->label << ">" << std::endl;
                    if( lx->label == this->label )
                    {
                        //std::cerr << "+TERM <" << label << ">" << std::endl;
                        xnode *term =  xnode::create(this->label,lx,semantic);
                        grow(Tree,term);
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
    
}

