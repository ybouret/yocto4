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
		stk_(),
		ctx_()
		{
			assert( root != NULL );
		}
		
		
		void parser:: restart( lexer &lxr, source &src ) throw()
		{
			lxr.drop( stk_, src ); //-- clear lexems
			ctx_.free();           //-- clear call tree
			on_restart();          //-- virtual call
		}
	
		void parser:: on_restart() throw() 
		{
			// do nothing
		}
		
	
		
		syntax::result parser:: operator()( lexer &lxr, source &src, const int flags)
		{
			return root_->match( lxr, src, stk_, flags, ctx_ );
		}

		
		
	}
	
	
}