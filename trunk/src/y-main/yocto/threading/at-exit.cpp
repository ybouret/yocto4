#include "yocto/threading/at-exit.hpp"
#include "yocto/threading/mutex.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include "yocto/code/swap.hpp"

namespace yocto
{
	
	namespace threading
	{
		
		namespace 
		{
			struct cb_t
			{
				at_exit::callback     procedure;
				threading::longevity  life_time;
			};
			
			static cb_t   cb_reg[ YOCTO_AT_EXIT_STACK_SIZE ];
			static size_t cb_num = 0;
			
			static void cb_all( void ) throw()
			{
				cb_t *cb = cb_reg;
				for( size_t i=cb_num; i>0; --i, ++cb )
				{
					assert( cb->procedure );
					try
					{
						cb->procedure();
					}
					catch(...)
					{
					}
					
				}
				cb_num = 0;
			}			
		}
		
		void at_exit::perform( at_exit::callback procedure, threading::longevity life_time)
		{
			static bool registered = false;
			
			//-- once registering
			YOCTO_GIANT_LOCK();
			if( !registered )
			{
				YOCTO_GIANT_LOCK();
				if( atexit( cb_all ) != 0 )
					throw libc::exception( errno, "atexit" );
				registered = true;
			}
			
			assert( procedure );
			
			//-- check room left
			if( cb_num >= YOCTO_AT_EXIT_STACK_SIZE )
				throw imported::exception( "YOCTO_AT_EXIT_STACK_SIZE too small", "reached #at_exit=%u", unsigned(YOCTO_AT_EXIT_STACK_SIZE) );
			
			//-- store at end of stack
			cb_t *curr      = &cb_reg[cb_num++];
			curr->procedure = procedure;
			curr->life_time = life_time;
			
			//-- re-order
			cb_t *prev = curr;
			while( --prev >= cb_reg && prev->life_time > curr->life_time )
			{
				mswap<cb_t>( *prev, *curr );
				--curr;
			}
			
		}
		
	}
	
}