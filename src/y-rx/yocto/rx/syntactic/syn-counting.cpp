#include "yocto/rx/syntactic/counting.hpp"
#include "yocto/auto-ptr.hpp"

#include <iostream>

namespace yocto
{
	namespace regex
	{
		
		namespace syntactic
		{
			
			counting:: ~counting() throw() {}
			
			counting:: counting( uint32_t t, const string &n, rule *r, size_t m ) :
			joker( t, n, r ),
			min_number( m )
			{
			}
			
			
			syntax::result counting::match( YOCTO_RX_SYNTAX_RULE_MATCH_ARGS )
			{
				std::cerr << "?<" << name << ".geq." << min_number << ">" << std::endl;
				check(tree);
				s_node *local_tree = s_node::create( *this );
				try
				{
					
					size_t num = 0;
					//-- try to match and accumulate
					for(;;)
					{
						const int res  =  jk->match(lxr,src,local_tree);
						if( res != syntax::success )
						{
							break;
						}
						++num;
					}
					
					// done
					if( num < min_number )
					{
						exception excp( "syntax mismatch", "#%s=%u<%u in ", name.c_str(), unsigned(num), unsigned(min_number));
						unwind( excp );
						throw excp;
					}
					
					// slight optimzation
					if( num > 0 )
					{
						if( tree ) 
							tree->append(local_tree);
						else 
							tree = local_tree;
					}
					else 
					{
						s_node::destroy(local_tree);
					}
					
					return syntax::success;
				}
				catch(...)
				{
					s_node::restore( local_tree, lxr.cache );
					throw;
				}
			}
			
			
			//==================================================================
			optional:: ~optional() throw() {}
			
			optional:: optional( const string &n, rule *r ) :
			counting( ID, n, r,0)
			{
			}
			
			optional * optional:: create( const string &n, rule *r )
			{
				try {
					return new optional( n, r );
				}
				catch (...) {
					delete r;
					throw;
				}
			}
			
			optional * optional:: create( const char *id, rule *r )
			{
				auto_ptr<rule> q( r );
				const string   n( id );
				
				return optional::create( n, q.yield() );
			}
			
			
			//==================================================================
			one_or_more:: ~one_or_more() throw() {}
			
			one_or_more:: one_or_more( const string &n, rule *r ) :
			counting( ID, n, r,1)
			{
			}
			
			one_or_more * one_or_more:: create( const string &n, rule *r )
			{
				try {
					return new one_or_more( n, r );
				}
				catch (...) {
					delete r;
					throw;
				}
			}
			
			one_or_more * one_or_more:: create( const char *id, rule *r )
			{
				auto_ptr<rule> q( r );
				const string   n( id );
				
				return one_or_more::create( n, q.yield() );
			}
			
			
			
		}
		
	}
	
}

