#include "yocto/threading/team.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/threading/thread.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		
		namespace 
		{
			class member
			{
			public:
				team   *simd;
				size_t  rank;
				thread  thr;
			};
		}
		
		void team:: check_ready() throw()
		{
			while(true)
			{
				if(!guard_.try_lock()) continue;
				const bool achieved = ready_ >= size;
				guard_.unlock();
				if( achieved ) break;
			}
		}
		
		void team:: terminate() throw()
		{
			std::cerr << "[team.terminate]" << std::endl;
			_stop_    = true;   //!< prepare shutdown
			start_.broadcast(); //!< wake up all threads
			member *addr = static_cast<member*> (wksp_);
			while( counter_ > 0 )
			{
				member &m = addr[--counter_];
				m.thr.join();
				m.thr.~thread();
			}
			memory::kind<memory::global>::release( wksp_, wlen_ );
			//std::cerr << "[team.terminated!]" << std::endl;
		}
		
		team:: ~team() throw()
		{
			//std::cerr << "[team.destroy]" << std::endl;
			terminate();
		}
		
		
		team:: team( size_t np ) :
		size( np <= 1 ? 1 : np ),
		_stop_( false  ),
		guard_( "team" ),
		start_(),
		final_(),
		ready_(0),
		active_(),
		task_(NULL),
		wlen_( size * sizeof(member) ),
		wksp_( memory::kind<memory::global>::acquire( wlen_ ) ),
		counter_(0)
		{
			try
			{
				
				member *m = static_cast<member*> (wksp_);
				while( counter_ < size )
				{
					m->simd = this;
					m->rank = counter_;
					new ( &(m->thr) ) thread( team::launcher, m );
					++m;
					++counter_;
				}
				
				
				check_ready();
				std::cerr << "[team.activated]" << std::endl;
				
			}
			catch(...)
			{
				terminate();
				throw;
			}
			
		}
		
		void team:: launcher( void *args ) throw()
		{
			assert( args );
			member *m = static_cast<member*>( args );
			assert( m->simd );
			assert( m->rank < m->simd->size );
			m->simd->engine( m->rank );
		}
		
		
		void team:: engine( size_t rank ) throw()
		{
			context ctx( rank, size, guard_);
			guard_.lock();
			//std::cerr << "[team.engine #" << rank << "]" << std::endl;
		CYCLE:
			//------------------------------------------------------------------
			// guard is locked
			//------------------------------------------------------------------
			assert( ready_ < size );
			++ready_;
			start_.wait( guard_ );
			//std::cerr << "\t[team:engine #" << rank << " starting...]" << std::endl;
			guard_.unlock();
			
			//------------------------------------------------------------------
			// Testing the internal _stop_ flag
			//------------------------------------------------------------------
			if( _stop_ )
			{
				//YOCTO_LOCK(guard_);
				//std::cerr << "\t[team.engine #" << rank << " stopped!]" << std::endl; 
				return;
			}
			
			//------------------------------------------------------------------
			// Do the work
			//------------------------------------------------------------------
			/*
			 {
			 YOCTO_LOCK(guard_);
			 std::cerr << "\t[team.working #" << rank << " ]" << std::endl;
			 }
			 */
			assert(task_);
			(*task_)(ctx);
			//------------------------------------------------------------------
			// Shall signal the main thread
			//------------------------------------------------------------------
			guard_.lock();
			assert(active_>0);
			if( --active_ <= 0 )
				final_.broadcast();
			goto CYCLE;
			
			
		}
		
		void team:: cycle( task &todo ) throw()
		{
			//------------------------------------------------------------------
			// synchronize the threads: wait for #ready==size
			//------------------------------------------------------------------
			check_ready();
			
			//------------------------------------------------------------------
			// prepare a loop
			//------------------------------------------------------------------
			guard_.lock();
			assert( ready_ ==  size   );
			assert( false  == _stop_  );
			ready_  =  0x00;
			active_ =  size;
			task_   = &todo;
			
			//------------------------------------------------------------------
			// prepare to go, guard is locked
			//------------------------------------------------------------------
			start_.broadcast();
			
			//------------------------------------------------------------------
			//  wait on final: unlock guard (everybody starts)
			//------------------------------------------------------------------
			final_.wait( guard_ );
			
			//------------------------------------------------------------------
			// At this point, everybody is done
			//------------------------------------------------------------------
			guard_.unlock();
		}
		
		
	}
	
}

