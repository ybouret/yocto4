#include "yocto/lang/lexeme.hpp"

namespace yocto 
{
    
    namespace lang
    {
        lexeme:: ~lexeme() throw() {}
        
        lexeme:: lexeme( const string &n, size_t l ) throw() :
        regex::token(),
        label(n),
        line(l),
        next(0),
        prev(0)
        {
        }
        
        
        lexeme:: lexeme( const lexeme &other ) :
        regex::token(other),
        label( other.label ),
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
