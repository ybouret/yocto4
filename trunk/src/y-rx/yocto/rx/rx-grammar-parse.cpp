#include "yocto/rx/grammar.hpp"
#include <iostream>

namespace yocto
{
	namespace regex
	{
		
		syntax::c_node *grammar:: parse( lexer &lxr, source &src )
		{
			//==================================================================
			//
			// initialize
			//
			//==================================================================
			if( rset_.size() <= 0 )
				throw exception("empty grammar '%s'", name.c_str());
			reset();
			syntax::rule     & root = ** rset_.begin();
			std::cerr << std::endl << "-- <" << name << ">.parsing from " << root.name << std::endl;
			
			//==================================================================
			//
			// run
			//
			//==================================================================
			const syntax_result res = root.match( lxr, src, tree_);
			switch( res )
			{
				case syntax_nothing:
					throw exception("End Of File for '%s'", root.name.c_str() );
					
				case syntax_unexpected:
					assert( lxr.cache.size  > 0 );
					switch( lxr.cache.size )
				{
					case 0:
						throw syntax::exception("Internal Unexpected Failure!","parsing <%s>", name.c_str());
					case 1:
						throw syntax::exception("Unexpected Lexeme", "Invalid '%s', in <%s>", lxr.cache.head->label.c_str(), name.c_str() );
						
					default:
						assert( lxr.cache.size>1 );
						throw syntax::exception("Unexpected Lexeme", "Invalid '%s' after '%s', in <%s>", lxr.cache.tail->label.c_str(), lxr.cache.tail->prev->label.c_str(), name.c_str());
						break;
				}
				default:
					break;
			}
			
			//==================================================================
			//
			// finalize
			//
			//==================================================================
			std::cerr << std::endl << "-- <" << name << ">.compiling tree '" << tree_->link.name << "'" << std::endl;
			tree_->compile();
			return tree_;
		}
		
	}
	
}

