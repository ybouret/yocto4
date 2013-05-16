#include "yocto/lingua/lexeme.hpp"

namespace yocto
{
	namespace lingua
	{
		lexeme:: ~lexeme() throw() {}
        
		lexeme:: lexeme( const lexeme &other ) :
		token(other),
        prev(0),
        next(0),
        label(other.label),
        line(other.line)
		{
            
		}
        
		lexeme:: lexeme( const string &id, size_t il ) throw() :
		token(),
        prev(0),
        next(0),
        label(id),
        line(il)
		{
            
		}
        
		lexeme * lexeme:: clone() const
		{
			return new lexeme( *this );
		}
	}
}
