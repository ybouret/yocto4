#include "yocto/rx/grammar.hpp"
#include <iostream>

namespace yocto
{
	namespace regex
	{
		
		void grammar:: reset() throw()
		{
			if( tree_ ) 
			{
				syntax::c_node::destroy( tree_ );
				tree_ = NULL;
			}
		}
		
		grammar:: ~grammar() throw()
		{
			reset();
		}
		
		grammar:: grammar( const string &n) : 
		name(n),
		tree_(NULL),
		rset_(8,as_capacity)
		{
			
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
		
		syntax::rule  * grammar:: operator[]( const char *txt )
		{
			const string id( txt );
			return (*this)[ id ];
		}
		
		cst_node *grammar:: parse( lexer &lxr, source &src, syntax_result &res )
		{
			//==================================================================
			//
			// initialize
			//
			//==================================================================
			if( rset_.size() <= 0 )
				throw exception("empty grammar '%s'", name.c_str());
			rules_set::iterator root = rset_.begin();
			reset();
			
			
			//==================================================================
			//
			// run
			//
			//==================================================================
			res = (*root)->match( lxr, src, tree_);
			return tree_;
		}
		
		
		void grammar:: record( syntax::rule *r )
		{
			syntax::rule::ptr p(r);
			if( !rset_.insert(p) )
			{
				throw syntax::exception( "grammar::record failure", "mutiple rule '%s'", p->name.c_str() ); 
			}
		}
		
		void grammar:: variant( const string &n )
		{
			syntax::terminal *r = new syntax::variant(n,*this);
			record(r);			
		}
		
		void grammar:: certain( const string &n )
		{
			syntax::terminal *r = new syntax::certain(n,*this);
			record(r);			
		}
		
		void grammar:: useless( const string &n )
		{
			syntax::terminal *r = new syntax::useless(n,*this);
			record(r);			
		}
		
		
		syntax::aggregate & grammar:: aggregate( const string &n )
		{
			syntax::aggregate *r = new syntax::aggregate(n,*this);
			record(r);			
			return *r;
		}
		
		syntax::alternative & grammar:: alternative( const string &n )
		{
			syntax::alternative *r = new syntax::alternative(n,*this);
			record(r);			
			return *r;
		}
		
		void grammar:: counting( const string &n, const string &p, char kind)
		{
			switch( kind )
			{
				case '?':
					record( new syntax::optional(n,*this,p) );
					break;
					
				case '+':
					record( new syntax::one_or_more(n,*this,p) );
					break;
					
				case '*':
					record( new syntax::any_count(n,*this,p) );
					break;
					
				default:
					throw syntax::exception("grammar::counting failure", "invalid kind '%c'", kind );
			}
		}
		
		
	}
}
