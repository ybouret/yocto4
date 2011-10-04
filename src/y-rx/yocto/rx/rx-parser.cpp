#include "yocto/rx/parser.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
#if 0
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
		
		
		void parser:: graphviz( ios::ostream &os, const string &graph_name ) const
		{
			os.append("digraph ");
			os.append( graph_name );
			os.append( " {\n");
			if( node_ )
			{
				node_->viz( os );
			}
			os.append( "}\n");
		}

#endif
		
	}
	
	
}