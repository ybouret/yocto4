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
		rset_(8,as_capacity) //,mset_(8,as_capacity)
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
		
				
		
		void grammar:: record( syntax::rule *r )
		{
			std::cerr << name << ".recording " << r->name << std::endl;
			syntax::rule::ptr p(r);
			if( !rset_.insert(p) )
			{
				throw syntax::exception( "grammar::record failure", "mutiple rule '%s'", p->name.c_str() ); 
			}
			
		}
		
		
		//----------------------------------------------------------------------
		// rules creation API
		//----------------------------------------------------------------------
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
		
		//----------------------------------------------------------------------
		// action
		//----------------------------------------------------------------------
#if 0
		void grammar:: operator()( const string &rule_name, const production &do_something )
		{
			if( ! mset_.insert( rule_name, do_something ) )
			{
				throw exception("%s.on(MUTLIPLE '%s')", name.c_str(), rule_name.c_str() );
			}
		}
		
		void grammar:: apply( const string &rule_name, const token &tkn )
		{
			production *a = mset_.search( rule_name );
			if( a )
			{
				(void) (*a)(rule_name,tkn);
			}
		}
#endif
		
	}
}
