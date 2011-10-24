#include "yocto/utest/run.hpp"

#include "yocto/threading/thread.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/round.hpp"
#include "yocto/ios/icstream.hpp"

#include <cstdlib>
#include <cstdio>

using namespace yocto;
using namespace threading;

namespace {
	
	class SIMD
	{
	public:
		static const size_t MAX_THREADS = 32;
		bool      run;
		mutex     guard;
		condition start;
		condition ready;
		condition reset;
		size_t    num_ready;
		size_t    num_done;
		struct Param
		{
			SIMD *simd;
		};
		const size_t  num_threads;
		Param         params[MAX_THREADS];
		uint64_t      wksp[ MAX_THREADS * YOCTO_U64_FOR_ITEM(thread) ];
		thread       *threads;
		size_t        counter;
		
		SIMD( size_t np ) : 
		run( true ),
		guard( "SIMD" ), 
		start(),
		ready(),
		reset(),
		num_ready(0),
		num_done(0),
		num_threads( clamp<size_t>(1,np,MAX_THREADS) ),
		params(),
		wksp(),
		threads( (thread *)&wksp[0] ),
		counter(0)
		{
			try 
			{
				while( counter < num_threads )
				{
					params[counter].simd = this;
					new (threads+counter) thread( SIMD::engine, & params[counter] );
					++counter;
				}
				guard.lock();
				std::cerr << "[SIMD.waiting to be ready]" << std::endl;
				ready.wait( guard );
				guard.unlock();
			}
			catch(...)
			{
				terminate();
				throw;
			}
		}
		
		
		
		void terminate() throw()
		{
			{
				YOCTO_LOCK(guard);
				std::cerr << "[SIMD.terminate]" << std::endl;
			}
			
			run = false;
			start.broadcast();
			while(counter>0)
			{
				--counter;
				threads[counter].join();
				threads[counter].~thread();
			}
		}
		
		
		~SIMD() throw()
		{
			{
				YOCTO_LOCK(guard);
				std::cerr <<  "[SIMD.destroy]"  << std::endl;
			}
			terminate();
		}
		
		static void engine( void *args ) throw()
		{
			Param *param = (Param *)args;
			assert(param->simd!=NULL);
			SIMD  &simd  = *(param->simd);
			{
				YOCTO_LOCK(simd.guard);
				std::cerr << "in thread..." << thread::get_current_id() << std::endl;
			}
			
			//------------------------------------------------------------------
			// Wait until everybody is ready
			//------------------------------------------------------------------
			while( true )
			{
				simd.guard.lock();
				assert( simd.num_ready < simd.num_threads );
				if( ++simd.num_ready >= simd.num_threads )
				{
					std::cerr << "[SIMD.is_ready]" << std::endl;
					simd.ready.signal();
					simd.num_ready = 0 ;
				}
				simd.start.wait( simd.guard );
				std::cerr << "Signaled..." << thread::get_current_id() << std::endl;
				const bool all_done = ++simd.num_done >= simd.num_threads;
				simd.guard.unlock();
				
#if 1
				if( !simd.run )
				{
					{
						YOCTO_LOCK(simd.guard);
						std::cerr << "Stopping..." << thread::get_current_id() << std::endl;
					}
					if( all_done ) simd.reset.signal();
					return;
				}
				
				{
					YOCTO_LOCK(simd.guard);
					std::cerr << "Computing..." << thread::get_current_id() << std::endl;
				}
				
				
				
				if( all_done )
					simd.reset.signal();
#endif
				
			}
			
		}
		
		void cycle()
		{
			{
				YOCTO_LOCK(guard);
				std::cerr << "[SIMD.cycle]" << std::endl;
			}
			guard.lock();
			start.broadcast();
			reset.wait(guard);
			guard.unlock();
			
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
	};
	
}

YOCTO_UNIT_TEST_IMPL(SIMD)
{
	size_t np = 2;
	if( argc > 1 ) np = atol( argv[1] );
	SIMD simd(np);
	std::cerr << "now in main: waiting for instructions:" << std::endl;
	fflush(stderr);
	ios::icstream in( ios::cstdin );
	string line;
	
	while( line.clear(), in.read_line(line) >= 0 )
	{
		if( line == 'c' )
		{
			simd.cycle();
			continue;
		}
		
		if( line == 'q' )
		{
			break;
		}
	}

	
}
YOCTO_UNIT_TEST_DONE()
