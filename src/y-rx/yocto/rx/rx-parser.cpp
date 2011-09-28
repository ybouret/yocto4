#include "yocto/rx/parser.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		parser:: ~parser() throw()
		{
		}

		parser:: parser( syntactic::rule *root ) throw() :
		root_( root ),
		stk_()
		{
			assert( root != NULL );
		}
		
		
		void parser:: restart( lexer &lxr, source &src ) throw()
		{
			stk_.to( src.char_pool );
			on_restart();          //-- virtual call
		}
	
		void parser:: on_restart() throw() 
		{
			// do nothing
		}
		
	
		
		syntax::result parser:: operator()( lexer &lxr, source &src)
		{
			return root_->match( lxr, src, stk_ );
		}

		
		
	}
	
	
}