#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	namespace regex
	{
		
		lexer:: ~lexer() throw()
		{
		}
		
		lexer:: lexer( label_type init ) :
		current_( NULL ),
		engines_( 4, as_capacity )
		{
			declare( init );
		}
		
		void lexer:: declare( label_type l ) 
		{
			
			const lexical::engine tmp( l );
			if( !engines_.insert( tmp ) )
			{
				throw exception( "lexer::declare(multiple label '%s')", tmp.name() );
			}
		}
	}
}
