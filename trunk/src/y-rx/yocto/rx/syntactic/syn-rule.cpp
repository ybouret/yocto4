#include "yocto/rx/syntactic/rule.hpp"

namespace yocto
{
	
	namespace regex
	{
		namespace syntactic
		{
			
			rule:: ~rule() throw() {}
			
			rule:: rule( uint32_t t, const string &id ) : 
			type(t),
			name( id ),
			next(NULL),
			prev(NULL)
			{}
			
			rules:: rules() throw() {}
			
			static inline 
			void __delete_rule( rule *r ) throw() { assert(NULL!=r); delete r; }
			
			rules:: ~rules() throw() { delete_with( __delete_rule ); }
			
			
			void rule:: unwind( const context  &ctx, exception &e ) throw()
			{
				for( size_t i=1; i <= ctx.size(); ++i )
				{
					e.cat( ".%s", ctx[i]->c_str() );
				}
			}
			
			syntax::result rule:: match(lexer       &lxr, 
										source      &src,
										lexemes     &stk,
										const int    flags, 
										context     &ctx )
			{
				//--------------------------------------------------------------
				// register it in the context
				//--------------------------------------------------------------
				ctx.push_back( & name );
#if !defined(NDEBUG)
				const size_t nlx = stk.size;
#endif
				
				//--------------------------------------------------------------
				// analyze the lexer 
				//--------------------------------------------------------------
				const syntax::result ans = analyze(lxr, src, stk);
				switch( ans )
				{
						//------------------------------------------------------
						// SUCCESS
						//------------------------------------------------------
					case syntax::success:
						//-- the lexeme(s) should be pushed onto the stack
						assert( stk.size >= nlx );
						ctx.pop_back(); //-- remove this from context
						return syntax::success;
						
						//------------------------------------------------------
						// NOTHING
						//------------------------------------------------------
					case syntax::nothing:
						//-- in any case no lexeme on the stack
						assert( nlx == stk.size );
						if( flags & int(syntax::nothing) )
						{
							
							ctx.pop_back(); //-- remove this from context
							return syntax::nothing;
						}
						else 
						{
							exception excp( "end of input", "at " );
							unwind( ctx, excp );
							throw excp;
						}
						break;
						
						//------------------------------------------------------
						// UNEXPECTED
						//------------------------------------------------------
					case syntax::unexpected:
						//-- in any case, no lexeme on the stack
						assert( nlx == stk.size );
						//-- but at least one lexeme in cache
						assert( lxr.cache.size > 0 );
						assert( NULL != lxr.cache.head );
						if( flags & int(syntax::unexpected) )
						{
							ctx.pop_back();
							return syntax::unexpected;
						}
						else 
						{
							const string tkn = lxr.cache.head->to_string();
							exception excp( "unexpected token", "'%s' at ", tkn.c_str() );
							unwind( ctx, excp );
							throw excp;
						}
						break;
						
						//------------------------------------------------------
						// ERROR
						//------------------------------------------------------
					case syntax::error:
						//-- in any case, no lexeme on the stack
						assert( nlx == stk.size ); {
							assert( src.peek() != NULL );
							exception excp( "invalid char", "'%c' at ", src.peek()->data );
							unwind( ctx, excp );
							throw excp;
						}
						break;
						
						
				}
				
				//--------------------------------------------------------------
				// UNKNOWN !!!
				//--------------------------------------------------------------
				{
					exception excp( "unknown analyzer result", "at " );
					unwind( ctx, excp );
					throw excp;
				}
			}
			
			
		}
		
	}
	
}
