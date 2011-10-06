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
		
		syntax::c_node *grammar:: parse( lexer &lxr, source &src )
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
			const syntax_result res = (*root)->match( lxr, src, tree_);
			switch( res )
			{
				case syntax_nothing:
					throw exception("End Of File for '%s'", (*root)->name.c_str() );
					
				case syntax_unexpected:
					assert( lxr.cache.size  > 0 );
					switch( lxr.cache.size )
				{
					case 0:
						throw syntax::exception("Internal Unexpected Failure!","parsing '%s'", name.c_str());
					case 1:
						throw syntax::exception("Unexpected Lexeme", "Invalid '%s'", lxr.cache.head->label.c_str() );
						
					default:
						assert( lxr.cache.size>1 );
						throw syntax::exception("Unexpected Lexeme", "Invalid '%s' after '%s'", lxr.cache.tail->label.c_str(), lxr.cache.tail->prev->label.c_str() );
						break;
				}
				default:
					break;
			}
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
		
		
		void grammar:: terminal( const string &n, int p)
		{
			syntax::terminal *r = new syntax::terminal(n,p,*this);
			record(r);			
		}
		
		
		
		syntax::aggregate & grammar:: aggregate( const string &n, int p)
		{
			syntax::aggregate *r = new syntax::aggregate(n,p,*this);
			record(r);			
			return *r;
		}
		
		syntax::alternative & grammar:: alternative( const string &n )
		{
			syntax::alternative *r = new syntax::alternative(n,*this);
			record(r);			
			return *r;
		}
		
		void grammar:: counting( const string &n, const string &m, char kind, int p)
		{
			switch( kind )
			{
				case '?':
					record( new syntax::optional(n,p,*this,m) );
					break;
					
				case '+':
					record( new syntax::one_or_more(n,p,*this,m) );
					break;
					
				case '*':
					record( new syntax::any_count(n,p,*this,m) );
					break;
					
				default:
					throw syntax::exception("grammar::counting failure", "invalid kind '%c'", kind );
			}
		}
		
		
	}
}
