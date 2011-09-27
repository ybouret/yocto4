#include "yocto/rx/syntactic/counting.hpp"
#include "yocto/auto-ptr.hpp"

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
			
			
			
			
			
			syntax::result counting::analyze( lexer &lxr, source &src, lexemes &stk, context &ctx)
			{
				lexemes   local_stk;
				size_t    num = 0;
				std::cerr << "'" << name << "' counting '" << jk->name << "'" << std::endl; 
				while( true )
				{
					lexemes              stk1;
					const syntax::result res = jk->match( lxr, src, stk1, ctx, int(syntax::unexpected) | int(syntax::nothing) );
					if( syntax::success != res )
					{
						assert( syntax::error != res );
						assert( 0 == stk1.size );
						break;
					}
					else 
					{
						++num;
						local_stk.merge_back( stk1 );
					}
				}
				std::cerr << "#num=" << num << std::endl;
				if( num < min_number )
				{
					lxr.unget( local_stk );
					exception excp( "invalid lexeme count","%u<%u in ", unsigned(num), unsigned(min_number) );
					unwind( ctx, excp );
					throw excp;
				}
				stk.merge_back( local_stk );
				return syntax::success;
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

