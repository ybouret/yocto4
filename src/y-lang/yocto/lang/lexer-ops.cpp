#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    
    namespace lang 
    {

        void lexer:: jump( const string &id )
        {
            assert( current != NULL );
            current = fetch(id);
        }
        
        
        lexeme * lexer:: next_lexeme( regex::source &src )
        {
            assert( current != NULL );
            
            if( cache.size > 0 )
                return cache.pop_front();

            return current->next_lexeme(src);
            
        }

        
    }

}
