#include "yocto/threading/at-exit.hpp"
#include "yocto/threading/mutex.hpp"
#include "yocto/error.hpp"
#include "yocto/code/bswap.hpp"

#include <cstdlib>
#include <cerrno>

//#include <iostream>

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
		
		void at_exit::perform(at_exit::callback    procedure,
                              threading::longevity life_time) throw()
		{
			static bool registered = false;
			
            //__________________________________________________________________
			//
            // once registering
            //__________________________________________________________________
			YOCTO_GIANT_LOCK();
			if( !registered )
			{
				YOCTO_GIANT_LOCK();
				if( atexit( cb_all ) != 0 )
					libc::critical_error( errno, "atexit" );
				registered = true;
			}
			
			assert( procedure );
            //std::cerr << "*** at_exit@" << (void *)procedure << " $" << life_time << std::endl;
            
            //__________________________________________________________________
            //
			// check room left
            //__________________________________________________________________
			if( cb_num >= YOCTO_AT_EXIT_STACK_SIZE )
				libc::critical_error( ERANGE, "YOCTO_AT_EXIT_STACK_SIZE overflow");
			
            //__________________________________________________________________
            //
			// store at end of stack
            //__________________________________________________________________
			cb_t *curr      = &cb_reg[cb_num++];
			curr->procedure = procedure;
			curr->life_time = life_time;
			
            //__________________________________________________________________
            //
			// re-order
            //__________________________________________________________________
			cb_t *prev = curr;
			while( --prev >= cb_reg && prev->life_time > curr->life_time )
			{
				bswap<cb_t>( *prev, *curr );
				--curr;
			}
			
		}
		
	}
	
}
