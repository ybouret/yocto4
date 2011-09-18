#include "yocto/rx/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
	namespace regex
	{
		
		lexer:: ~lexer() throw()
		{
			rules.delete_with( lexical::rule::destroy );
		}
		
		lexer:: lexer() throw() :
		rules()
		{
		}
		
		
		void lexer:: operator()( pattern *motif, const string &label )
		{
			assert( motif );

			for( const lexical::rule *r = rules.tail; r; r=r->prev )
			{
				if( r->label == label )
				{
					delete motif;
					throw exception( "lexer(MULTIPLE '%s')", label.c_str() );
				}
			}
			
			rules.push_back(  lexical::rule::create( motif, label ) );

		}

		void lexer:: operator()( pattern *motif, const char *label )
		{
			auto_ptr<pattern> p( motif );
			const string      l( label );
			
			(*this)( p.yield(),l );
		}
		
		
		void lexer:: reset( source &src ) throw()
		{
			for( lexical::rule *r = rules.tail; r; r=r->prev)
			{
				r->motif->sendto( src );
			}
		}
				
	}
}
