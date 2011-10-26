#include "yocto/threading/simd.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/threading/thread.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		
		namespace 
		{
			class Unit
			{
			public:
				SIMD   *simd;
				size_t  rank;
				thread  thr;
			};
		}
		
		void SIMD:: check_ready() throw()
		{
			while(true)
			{
				if(!guard_.try_lock()) continue;
				const bool achieved = ready_ >= threads;
				guard_.unlock();
				if( achieved ) break;
			}
		}
		
		void SIMD:: terminate() throw()
		{
			std::cerr << "[SIMD.terminate]" << std::endl;
			_stop_    = true; //!< prepare shutdown
			start_.broadcast(); //!< wake up all threads
			Unit *unit = static_cast<Unit*> (wksp_);
			while( counter_ > 0 )
			{
				Unit &u = unit[--counter_];
				u.thr.join();
				u.thr.~thread();
			}
			memory::kind<memory::global>::release( wksp_, wlen_ );
			std::cerr << "[SIMD.terminated!]" << std::endl;
		}
		
		SIMD:: ~SIMD() throw()
		{
			std::cerr << "[SIMD.destroy]" << std::endl;
			terminate();
		}
		
		
		SIMD:: SIMD( size_t np ) :
		threads( np <= 1 ? 1 : np ),
		_stop_( false  ),
		guard_( "SIMD" ),
		start_(),
		final_(),
		ready_(0),
		active_(),
		proc_(NULL),
		idle( this, & SIMD::idle_ ),
		wlen_( threads * sizeof(Unit) ),
		wksp_( memory::kind<memory::global>::acquire( wlen_ ) ),
		counter_(0)
		{
			try
			{
				
				Unit *unit = static_cast<Unit*> (wksp_);
				while( counter_ < threads )
				{
					unit->simd = this;
					unit->rank = counter_;
					new ( &(unit->thr) ) thread( SIMD::CEngine, unit );
					++unit;
					++counter_;
				}
				
				
				check_ready();
				std::cerr << "[SIMD.Achieved]" << std::endl;
				
			}
			catch(...)
			{
				terminate();
				throw;
			}
			
		}
		
		void SIMD::CEngine( void *args ) throw()
		{
			assert( args );
			Unit *unit = static_cast<Unit*>( args );
			assert( unit->simd );
			assert( unit->rank < unit->simd->threads );
			unit->simd->engine( unit->rank );
		}
		
		
		void SIMD:: engine( size_t rank ) throw()
		{
			guard_.lock();
			std::cerr << "[SIMD.engine #" << rank << "]" << std::endl;
		CYCLE:
			//------------------------------------------------------------------
			// guard is locked
			//------------------------------------------------------------------
			assert( ready_ < threads );
			++ready_;
			start_.wait( guard_ );
			std::cerr << "\t[SIMD:engine #" << rank << " starting...]" << std::endl;
			guard_.unlock();
			
			//------------------------------------------------------------------
			// Testing the internal _stop_ flag
			//------------------------------------------------------------------
			if( _stop_ )
			{
				YOCTO_LOCK(guard_);
				std::cerr << "\t[SIMD.engine #" << rank << " stopped!]" << std::endl; 
				return;
			}
			
			//------------------------------------------------------------------
			// Do the work
			//------------------------------------------------------------------
			{
				YOCTO_LOCK(guard_);
				std::cerr << "\t[SIMD.working #" << rank << " ]" << std::endl;
			}
			assert(proc_);
			(*proc_)(rank);
			//------------------------------------------------------------------
			// Shall signal the main thread
			//------------------------------------------------------------------
			guard_.lock();
			assert(active_>0);
			if( --active_ <= 0 )
				final_.broadcast();
			goto CYCLE;
			
			
		}
		
		void SIMD:: cycle( Proc *proc ) throw()
		{
			assert( proc != NULL );
			check_ready();
			guard_.lock();
			assert( ready_ == threads );
			assert( false  == _stop_  );
			ready_  = 0;
			active_ = threads;
			proc_   = proc;
			std::cerr << "[SIMD.enter cycle]" << std::endl;
			start_.broadcast();
			final_.wait( guard_ );
			std::cerr << "[SIMD.leave cycle]" << std::endl;
			guard_.unlock();
		}
	
		void SIMD:: idle_(size_t rank) throw()
		{
			YOCTO_LOCK(guard_);
			std::cerr << "\t[SIMD.idle #" << rank << "]" << std::endl;
		}
		
	}
	
}

