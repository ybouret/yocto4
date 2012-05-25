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
            
            if( cache.size > 0 )
                return cache.pop_front();

            return scan->next_lexeme(src);
        }

        
    }

}
