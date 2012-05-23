#include "yocto/lexical/scanner.hpp"

namespace yocto 
{
    namespace lexical
    {
        scanner:: ~scanner() throw()
        {
        }
        
        
        scanner:: scanner():
        line(0),
        rules_(),
        cache_(),
        newline( this, & scanner::on_newline)
        {
        }
        
        bool scanner:: on_newline( const regex::token &) throw()
        {
            ++line;
            return false;
        }
    }
}
