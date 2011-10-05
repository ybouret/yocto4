#include "yocto/rx/syntax/counting.hpp"
#include "yocto/rx/grammar.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace syntax
		{
			
			////////////////////////////////////////////////////////////////////
			counting:: ~counting() throw()
			{
			}
			
			counting:: counting( uint32_t t, const string &n, grammar &g, const string &p ) :
			rule(t,n,g),
			motif( g[p] )
			{
				
			}
			
			
			syntax_result counting:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				std::cerr << "? COUNT " << name << " #" << motif->name << std::endl;
				size_t        num = 0;
				while( true )
				{
					const syntax_result res= motif->match(lxr,src);
					if( res != syntax_success )
					{
						break;
					}
					++num;
				}
				if( !is_valid( num ) )
				{
					throw exception("count mismatch","in #'%s'=%u", name.c_str(), unsigned(num) ); 
				}
				std::cerr << "+ COUNT " << name << " #" << motif->name << "= " << num << std::endl;
				return syntax_success;
			}

			
			////////////////////////////////////////////////////////////////////
			optional:: ~optional() throw() {}
			
			optional:: optional( const string &n, grammar &g, const string &p) :
			counting( ID,n,g, p )
			{
			}
			
			bool optional:: is_valid( size_t n ) const throw()
			{
				return (n == 0) || (n == 1);
			}
			
			////////////////////////////////////////////////////////////////////
			any_count:: ~any_count() throw() {}
			
			any_count:: any_count( const string &n, grammar &g, const string &p) :
			counting( ID,n,g, p )
			{
			}
			
			bool any_count:: is_valid( size_t n ) const throw()
			{
				return n >= 0;
			}
			
			////////////////////////////////////////////////////////////////////
			one_or_more:: ~one_or_more() throw() {}
			
			one_or_more:: one_or_more( const string &n, grammar &g, const string &p) :
			counting( ID,n,g, p )
			{
			}
			
			bool one_or_more:: is_valid( size_t n ) const throw()
			{
				return n>0;
			}
			
			
		}

	}

}
