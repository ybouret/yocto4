#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/rx/source.hpp"

namespace yocto
{
	namespace regex
	{
		
		lexer:: ~lexer() throw()
		{
			rules_.delete_with( lexical::rule::destroy );
		}
		
		lexer:: lexer() throw() :
		rules_(),
		cache()
		{
		}
		
		
		void lexer:: operator()( pattern *motif, const string &label )
		{
			assert( motif );

			for( const lexical::rule *r = rules_.tail; r; r=r->prev )
			{
				if( r->label == label )
				{
					delete motif;
					throw exception( "lexer(MULTIPLE '%s')", label.c_str() );
				}
			}
			
			rules_.push_back(  lexical::rule::create( motif, label ) );

		}

		void lexer:: operator()( pattern *motif, const char *label )
		{
			auto_ptr<pattern> p( motif );
			const string      l( label );
			
			(*this)( p.yield(),l );
		}
		
		
		void lexer:: reset( source &src ) throw()
		{
			for( lexical::rule *r = rules_.tail; r; r=r->prev)
			{
				r->motif->sendto( src );
			}
			cache.to( src.char_pool );
		}
		
		void lexer:: unget( lexeme *lx ) throw()
		{
			assert( NULL != lx );
			assert( NULL == lx->next );
			assert( NULL == lx->prev );
			cache.push_front( lx );
		}
				
		void lexer:: unget( lexemes &lxs ) throw()
		{
			while( lxs.size ) cache.push_front( lxs.pop_back() );
		}
		
	
				
	}
}
