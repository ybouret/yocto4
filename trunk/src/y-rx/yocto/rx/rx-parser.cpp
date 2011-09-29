#include "yocto/rx/parser.hpp"

namespace yocto
{
	
	namespace regex
	{
		void parser:: release() throw()
		{
			if( node_ )
			{
				syntactic::s_node::destroy( node_ );
				node_ = NULL;
			}
		}
		
		parser:: ~parser() throw()
		{
			release();
		}
		
		parser:: parser( syntactic::rule *root ) throw() :
		root_( root ),
		node_(NULL)
		{
			assert( root != NULL );
		}
		
		
		void parser:: restart( lexer &lxr, source &src ) throw()
		{
			if( node_ ) 
			{
				syntactic::s_node::destroy( node_, src.char_pool );
				node_ = NULL;
			}
			on_restart();          //-- virtual call
		}
		
		void parser:: on_restart() throw() 
		{
			// do nothing
		}
		
		
		
		syntax::result parser:: operator()( lexer &lxr, source &src)
		{
			return root_->match( lxr, src, node_ );
		}
		
		
		
	}
	
	
}