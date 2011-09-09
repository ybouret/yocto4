#include "yocto/rx/lexical/engine.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace lexical
		{
			
			//-- basic rule
			rule::  rule() throw() : next(0), prev(0) {}
			rule:: ~rule() throw() {}
			
			
			//-- make someting uppon pattern
			make:: ~make() throw() {}
			make::  make( const action &a ) : todo_( a ) {}
			
			rule *make:: clone() const { return new make( todo_ ); }
			void  make:: apply( const token &p )
			{
				todo_( p );
			}
			
			rule *make:: create( const action &a ) { return new make( a ); }
		
			//-- stop uppon pattern
			stop:: ~stop() throw() {}
			stop:: stop( engine &e, const action &a ) :
			engine_( e ), 
			onStop_( a ) 
			{}
			
			rule * stop:: create( engine &e, const action &a )
			{
				return new stop( e, a );
			}
		
			void stop:: apply( const token &t )
			{
				engine_.stop_ = true;
				onStop_(t);
			}
			
			rule * stop:: clone() const { return new stop( engine_, onStop_ ); }
			
		}
		
	}
}
