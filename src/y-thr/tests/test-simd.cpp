#include "yocto/utest/run.hpp"

#include "yocto/threading/thread.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/barrier.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/round.hpp"
#include "yocto/ios/icstream.hpp"

#include "yocto/wtime.hpp"
#include "yocto/code/rand.hpp"

#include <cstdlib>
#include <cstdio>

using namespace yocto;
using namespace threading;

namespace {
	
	class SIMD
	{
	public:
		static const size_t MAX_THREADS = 32;
		
		struct Param
		{
			SIMD  *simd;
			size_t indx;
		};
		
		bool          run;
		mutex         guard;
		condition     start;
		size_t        num_start;
		size_t        num_done;
		const size_t  num_threads;
		condition     done;
		Param         params[MAX_THREADS];
		uint64_t      wksp[ MAX_THREADS * YOCTO_U64_FOR_ITEM(thread) ];
		thread       *threads;
		size_t        counter;
		size_t        num_cycles;
		bool          do_work;
		
		SIMD( size_t np ) :
		run( true ),
		guard( "SIMD" ),
		start(),
		num_start(0),
		num_done(0),
		num_threads( clamp<size_t>(1,np,MAX_THREADS) ),
		done(),
		params(),
		wksp(),
		threads( (thread *)&wksp[0] ),
		counter(0),
		num_cycles(0),
		do_work(false)
		{
			try 
			{
				while( counter < num_threads )
				{
					Param *p = & params[counter];
					p->simd = this;
					p->indx = counter;
					new (threads+counter) thread( SIMD::Engine, p );
					++counter;
				}
				
				guard.lock();
				std::cerr << "[SIMD.waiting to be ready]" << std::endl;
				start.wait( guard );
				guard.unlock();
				
				{
					YOCTO_LOCK(guard);
					std::cerr << "[SIMD.constructed]" << std::endl;
				}
				
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
		
		
		static void Engine( void *args ) throw()
		{
			Param *param = (Param *)args; assert(param);
			param->simd->engine( param->indx );
		}
		
		
		void engine( size_t indx ) throw()
		{
			{
				YOCTO_LOCK(guard);
				std::cerr << "+engine #" << indx << std::endl;
			}
			
			//==================================================================
			//
			// Wait for everyone to be constructed
			//
			//==================================================================
			guard.lock();
			if( ++num_start >= num_threads )
			{
				num_start = 0;
				std::cerr << "[SIMD.all engines started]" << std::endl;
				start.broadcast();
			}
			else
				start.wait( guard );
			guard.unlock();
			
			
			//==================================================================
			//
			// Wait for Cycle|End
			//
			//==================================================================
			guard.lock();
		CYCLE:
			//std::cerr << "\tengine #" << indx << " is waiting" << std::endl;
			start.wait( guard );
			//std::cerr << "\tengine #" << indx << " shall work!" << std::endl;
			guard.unlock();
			
			if( !run )
			{
				// a call from terminate
				return;
			}
			
			// do some work
			if(do_work)
			{
				guard.lock();
				const double tmx = 0.1 * alea<double>();
				guard.unlock();
				wtime::sleep( tmx );
			}
			
			// end
			guard.lock();
			++num_done;
			std::cerr << "\tnum_done=" << num_done << "/engine #" << indx << std::endl;
			if( num_done >= num_threads )
				done.signal();
			//guard.unlock();
			goto CYCLE;
		}
		
		void cycle() throw()
		{
			++num_cycles;
			guard.lock();
			std::cerr << "[SIMD.cycle " << num_cycles << " ]" << std::endl;
			num_done = 0;
			start.broadcast();
			done.wait(guard);
			guard.unlock();
		}
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
	};
	
}

YOCTO_UNIT_TEST_IMPL(SIMD)
{
	size_t np = 2;
	size_t nc = 2;
	if( argc > 1 ) np = atol( argv[1] );
	if( argc > 2 ) nc = atol( argv[2] );
	std::cerr << ">>>> SIMD " << np << " " << nc << std::endl;
	SIMD simd(np);
	std::cerr << "Waiting for instructions:" << std::endl;
	fflush(stderr);
	ios::icstream in( ios::cstdin );
	string line;
	
	while( line.clear(), fprintf(stderr,">"), fflush(stderr), in.read_line(line) >= 0 )
	{
		if( line == 'c' )
		{
			for( size_t i=0; i < nc; ++i )
			{
				std::cerr << "[SIMD.enter cyle]" << std::endl;
				simd.do_work = alea<double>() > 0.5;
				simd.cycle();
				std::cerr << "[SIMD.leave cyle]" << std::endl;
			}
			continue;
		}
		
		if( line == 'q' )
		{
			break;
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
