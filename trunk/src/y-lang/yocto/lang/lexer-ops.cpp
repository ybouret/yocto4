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
    }

}
