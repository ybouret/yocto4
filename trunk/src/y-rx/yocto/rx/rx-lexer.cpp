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
			no_dict();
		}
		
		lexer:: lexer() throw() :
		rules_(),
		dict_(NULL),
		cache()
		{
		}
		
		
		void lexer:: operator()( pattern *motif, const string &label, const lexical::action *a)
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
			
			rules_.push_back(  lexical::rule::create( motif, label,a ) );

		}

		void lexer:: operator()( pattern *motif, const char *label, const lexical::action *a)
		{
			auto_ptr<pattern> p( motif );
			const string      l( label );
			
			(*this)( p.yield(),l,a);
		}
		
		
		void lexer:: reset() throw()
		{
			for( lexical::rule *r = rules_.tail; r; r=r->prev)
			{
				r->motif->clear();
			}
			cache.clear();
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
