#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace lang 
    {
        
        void lexer:: jump( const string &id )
        {
            assert( scan != NULL );
            scan = fetch(id);
            std::cerr << "[jump lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        void lexer:: call( const string &id )
        {
            assert(scan != NULL);
            lexical::scanner *next_scan = fetch(id);
            history.push(scan);
            scan = next_scan;
            std::cerr << "[call lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        void lexer:: back()
        {
            assert( scan != NULL );
            if( history.size() <= 0 ) throw exception("%s: can't go back!", scan->name.c_str() );
            scan = history.peek();
            history.pop();
            std::cerr << "[back lexer:scan] '" << scan->name << "'" << std::endl;
        }
        
        lexeme * lexer:: next_lexeme( regex::source &src )
        {
            assert( scan != NULL );
            
            
            
            while(true)
            {
                if( cache.size > 0 )
                    return cache.pop_front();
                
                bool    fctl = false;
                lexeme *lx   = scan->next_lexeme(src,fctl);
                
                //--------------------------------------------------------------
                //-- do we have something ?
                //--------------------------------------------------------------
                if(lx) 
                {
                    assert(false==fctl);
                    return lx;
                }
                
                //--------------------------------------------------------------
                //-- or do we have a control lexeme ?
                //--------------------------------------------------------------
                if( !fctl )
                {
                    //-- no: this is the end
                    break;
                }
            }
            return NULL;
        }
        
        void  lexer::unget( const lexical::scanner &from, const string &data )
        {
            //-- create an empty lexeme
            lexeme *lx = new lexeme( from.name, line );
            
            //-- store it
            cache.push_front(lx);
            try 
            {
                //-- create a corresponding token
                regex::token tmp( data );
                
                //-- steal it
                lx->swap_with(tmp);
            }
            catch(...)
            {
                delete cache.pop_front();
                throw;
            }
            
        }
        
    }
    
}
