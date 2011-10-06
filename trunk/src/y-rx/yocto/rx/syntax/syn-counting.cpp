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
			
			counting:: counting( uint32_t t, const string &n, int p, grammar &g, const string &m ) :
			rule(t,n,p | node_fusion,g),
			motif( g[m] )
			{
				
			}
			
			
			syntax_result counting:: match( YOCTO_RX_SYNTAX_RULE_ARGS )
			{
				std::cerr << "? COUNT " << name << " #" << motif->name << std::endl;
				size_t        num = 0;
				c_node *local_tree = c_node::create( *this );
				try
				{
					while( true )
					{
						const syntax_result res= motif->match(lxr,src,local_tree);
						if( res != syntax_success )
						{
							break;
						}
						++num;
					}
					if( !is_valid( num ) )
					{
						c_node::restore(local_tree,lxr);
						throw exception("count mismatch","in #'%s'=%u", name.c_str(), unsigned(num) ); 
					}
					std::cerr << "+ COUNT " << name << " #" << motif->name << "= " << num << std::endl;
					if( num > 0 )
					{
						c_node::append(tree,local_tree);
					}
					else 
					{
						c_node:: destroy( local_tree );
					}

					return syntax_success;
				}
				catch(...)
				{
					c_node::destroy( local_tree );
					throw;
				}
			}
			
			
			////////////////////////////////////////////////////////////////////
			optional:: ~optional() throw() {}
			
			optional:: optional( const string &n, int p, grammar &g, const string &m) :
			counting( ID,n, p, g,m )
			{
			}
			
			bool optional:: is_valid( size_t n ) const throw()
			{
				return (n == 0) || (n == 1);
			}
			
			////////////////////////////////////////////////////////////////////
			any_count:: ~any_count() throw() {}
			
			any_count:: any_count( const string &n, int p, grammar &g, const string &m) :
			counting( ID,n,p,g,m )
			{
			}
			
			bool any_count:: is_valid( size_t n ) const throw()
			{
				return n >= 0;
			}
			
			////////////////////////////////////////////////////////////////////
			one_or_more:: ~one_or_more() throw() {}
			
			one_or_more:: one_or_more( const string &n, int p, grammar &g, const string &m) :
			counting( ID,n,p,g,m )
			{
			}
			
			bool one_or_more:: is_valid( size_t n ) const throw()
			{
				return n>0;
			}
			
			
		}
		
	}
	
}
