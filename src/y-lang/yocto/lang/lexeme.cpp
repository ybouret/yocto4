#include "yocto/lang/lexeme.hpp"

namespace yocto 
{
    
    namespace lang
    {
        lexeme:: ~lexeme() throw() {}
        
        lexeme:: lexeme( const string &n, size_t l ) throw() :
        regex::token(),
        name(n),
        line(l),
        next(0),
        prev(0)
        {
        }
        
        
        lexeme:: lexeme( const lexeme &other ) :
        regex::token(other),
        name( other.name ),
        line( other.line ),
        next(0),
        prev(0)
        {
        }
        
        
        lexeme * lexeme:: clone() const
        {
            return new lexeme(*this);
        }
               
      
        
        
    }
}
