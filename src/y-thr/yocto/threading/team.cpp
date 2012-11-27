#include "yocto/threading/team.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
        void team:: split( size_t &shift, size_t &count, size_t huge, size_t rank, size_t size) throw()
        {
            assert(size>0);
            assert(rank<size);
            size_t done = 0;
            size_t todo = huge;
            for(size_t i=0; i < rank; ++i )
            {
                todo = huge / (size-i);
                huge -= todo;
                done += todo;
            }
            shift = done;
            count = todo;
        }
        
#if 0
        void team:: split( size_t *counts, size_t num_counts, size_t huge ) throw()
        {
            assert( !(0==counts&&num_counts>0));
            for( size_t rank = 0; rank < num_counts; ++rank )
            {
                const size_t todo = huge/(num_counts-rank);
                counts[rank] = todo;
                huge -= todo;
            }
        }
        
        size_t divide( size_t huge, size_t rank, size_t size ) throw()
        {
            assert(size>0);
            assert(rank<size);
            for(size_t i=0;i<size;++i)
            {
                const size_t todo = huge/(size-i);
                if( rank == i )
                    return todo;
                huge -= todo;
            }
            return 0;
        }
#endif
        
		//======================================================================
		//
		// team::context
		//
		//======================================================================
		team::context:: context( const size_t thread_id, const size_t num_threads, lockable &guard ) throw() :
		rank( thread_id   ),
		size( num_threads ),
		access( guard )
		{}
		
		team::context:: ~context() throw()
		{
		}
		
        
		//======================================================================
		//
		// team members
		//
		//======================================================================
#include "./team-member.hxx"
		
		
		
		//======================================================================
		//
		// team itself
		//
		//======================================================================
		void team:: check_ready() throw()
		{
			bool achieved = false;
			do
			{
				while(!guard_.try_lock());
				achieved = ready_ >= size;
				guard_.unlock();
			} while( !achieved );
			
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
		}
		
		team:: ~team() throw()
		{
			terminate();
		}
        
        void team:: initialize()
        {
            try
			{
				//--------------------------------------------------------------
                // create the threads
                //--------------------------------------------------------------
				member *m = static_cast<member*> (wksp_);
				while( counter_ < size )
				{
					m->simd = this;
					m->rank = counter_;
					new ( &(m->thr) ) thread( team::launcher, m );
					++m;
					++counter_;
				}
				
				//--------------------------------------------------------------
                // first wait to be ready
                //--------------------------------------------------------------
				check_ready();
				std::cerr << "[team.activated]" << std::endl;
                
                //--------------------------------------------------------------
                // set affinity if possible
                //--------------------------------------------------------------
				place();
			}
			catch(...)
			{
				terminate();
				throw;
			}
            
        }
        
        
#define Y_TEAM_CTOR()           \
_stop_( false    ),             \
guard_( mutex_id ),             \
start_(),                       \
final_(),                       \
ready_(0),                      \
active_(),                      \
task_(NULL),                    \
wlen_( size * sizeof(member) ), \
wksp_( memory::kind<memory::global>::acquire( wlen_ ) ), \
counter_(0)
        
        team:: team( size_t num_threads, size_t thread_offset, const char *mutex_id ) :
        layout(num_threads,thread_offset),
        Y_TEAM_CTOR()
        {
            initialize();
        }
        
		team:: team( const char *mutex_id) :
		layout(),
		Y_TEAM_CTOR()
		{
            initialize();
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
				return;
			}
			
			//------------------------------------------------------------------
			// Do the work
			//------------------------------------------------------------------
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

