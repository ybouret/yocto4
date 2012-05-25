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
        }
        
        void lexer:: call( const string &id )
        {
            assert(scan != NULL);
            lexical::scanner *next_scan = fetch(id);
            history.push(scan);
            scan = next_scan;
        }
        
        lexeme * lexer:: next_lexeme( regex::source &src )
        {
            assert( scan != NULL );
            
            if( cache.size > 0 )
                return cache.pop_front();

            return scan->next_lexeme(src);
        }

        
    }

}
