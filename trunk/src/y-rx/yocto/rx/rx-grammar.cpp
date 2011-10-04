#include "yocto/rx/grammar.hpp"

namespace yocto
{
	namespace regex
	{
		
		grammar:: ~grammar() throw() {}
		
		grammar:: grammar( syntax::rule *root, const string &n) : 
		root_( root ),
		name(n),
		rset_(8,as_capacity)
		{
			if( ! rset_.insert(root_ ) )
				throw syntax::exception( "grammar constructor", "unexpected failure for '%s'", root_->name.c_str() );
		}
		
		grammar & grammar:: operator<<( syntax::rule *r )
		{
			const syntax::rule::ptr p( r );
			if( ! rset_.insert(p) )
			{
				throw syntax::exception( "grammar setup","multiple rule '%s'", r->name.c_str() );
			}
			return *this;
		}
		
		syntax::rule  * grammar:: operator[]( const string &id )
		{
			syntax::rule::ptr *p = rset_.search( id );
			if( !p )
			{
				throw syntax::exception( "grammar lookup","no rule '%s'",id.c_str() );
			}
			return p->__get();
		}
		
		syntax_result grammar:: parse( lexer &lxr, source &src )
		{
			const  syntax_result res = root_->match( lxr, src );
			return res;
		}
		
	}
}