#include "yocto/threading/crew.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/wtime.hpp"

#include <iostream>

namespace yocto
{
	
	namespace threading
	{
		
		crew:: context:: ~context() throw()
		{
		}
		
		crew:: context:: context( size_t r, size_t n, mutex &m) throw() :
		rank( r ),
		size( n ),
		critical( m )
		{
		}
		
		
		
		crew:: ~crew() throw()
		{
			terminate();
		}
		
		mutex & crew:: critical() throw()
		{
			return guard_;
		}
		
		
		crew:: crew( size_t n ) :
		size( max_of<size_t>(1,n) ),
		proc( NULL ),
		guard_( "[crew]" ),
		start_( ),
		cycle_( ),
		fence_( size, "[crew.barrier]" ),
		valid_( true ),
		agent_( memory::kind<memory::global>::acquire_as<worker>( (size_t &)size) ),
		count_( 0 ),
		ready_( 0 )
		{
			//------------------------------------------------------------------
			//
			// create threads
			//
			//------------------------------------------------------------------
			std::cerr << "crew: creating " << size << " thread(s)" << std::endl;
			while( count_ < size )
			{
				worker      &w         = agent_[ count_ ]; //!< get the agent
				*(size_t *) &w.rank    = count_;           //!< its id
				w.crew_                = this;             //!< and a common boss
				
#if 0
				{
					scoped_lock _lock( guard_ );
					std::cerr << "crew: worker #" << w.rank << ": creating " << std::endl;
				}
#endif
				
				//const YCE_Error err = YCE_ThreadInit( & w.thread_ );
				//if( err )
				//{
				//	terminate();
				//	throw os_exception(err,"crew.YCE_ThreadInit");
				//}
				
				try
				{
					new ( & w.thread_ ) thread( crew::engine, &w );
				}
				catch(...)
				{
					terminate();
					throw;
				}
				
				
#if 0
				{
					scoped_lock _lock( guard_ );
					std::cerr << "crew: worker #" << w.rank << ": ok!" << std::endl;
				}
#endif
				++count_;
			}
			
			//------------------------------------------------------------------
			//
			// Wait for all threads to be waiting
			//
			//------------------------------------------------------------------
			check_ready();
			
			
		}
		
		
		void crew:: check_ready() throw()
		{
			bool match = false;
			do
			{
				while( ! guard_.try_lock() )
				{
					//timing::watch::sleep( 0.001 );
				}
				match = (ready_ >= count_);
				guard_.unlock();
			}
			while( !match );
		}
		
		void crew:: engine( void *args ) throw()
		{
			assert( args );
			worker  &w = *(worker *)args;
			assert( w.crew_ );
			
			//------------------------------------------------------------------
			//
			// gather variables
			//
			//------------------------------------------------------------------
			crew         &c     = *(w.crew_);
			condition    &start = c.start_; 
			condition    &cycle = c.cycle_;
			mutex        &guard = c.guard_;
			bool         &valid = c.valid_;
			size_t       &ready = c.ready_;
			barrier      &fence = c.fence_;
			//const size_t &count = c.count_;
			context ctx( w.rank, c.size, guard );
			
			
		CYCLE:
			{
				//--------------------------------------------------------------
				//
				// wait on the start condition
				//
				//--------------------------------------------------------------
				guard.lock();
				++ready;
				//std::cerr << "crew: worker #" << w.rank << " waiting...(" << ready << "/" << count << ")" << std::endl;
				//if( ready >= count ) std::cerr << "crew: everybody is ready !" << std::endl;
				start.wait( guard );
				guard.unlock();
				
				//--------------------------------------------------------------
				//
				// do we have work ?
				//
				//--------------------------------------------------------------
				if( ! valid )
				{
					guard.lock();
					assert( ready > 0 );
					--ready;
					guard.unlock();
					//std::cerr << "crew: worker #" << w.rank << " done...(" << ready << " remaining )" << std::endl;
					return;
				}
				
				//--------------------------------------------------------------
				//
				// perform work
				//
				//--------------------------------------------------------------
				{ 
					//scoped_lock _lock( guard );
					//std::cerr << "crew: worker #" << w.rank << " working..." << std::endl;
				}
				
				assert( c.proc );
				c.proc( ctx, w.args );
				
				//--------------------------------------------------------------
				//
				// wait on fence_ for main thread synchro
				//
				//--------------------------------------------------------------
				{ 
					//scoped_lock _lock( guard );
					//std::cerr << "crew.worker #" << w.rank << " synchronizing..." << std::endl;
				}
				
				guard.lock();
				assert( ready > 0 );
				--ready;
				guard.unlock();
				
				if( fence.wait() )
				{
					//-- this is the thread that reaches the threshold !
					//scoped_lock _lock( guard );
					//std::cerr << "crew.worker #" << w.rank <<  " last call !" << std::endl;
					//ready = 0;
					cycle.broadcast();
				}
				
				
				goto CYCLE;
			}
			
		}
		
		
		void crew:: terminate() throw()
		{
			assert( size == count_ );
			check_ready();
			std::cerr << "crew: terminate.broadcast" << std::endl;
			
			
			valid_ = false;
			//-- cycle until every one got the message
			for(;;)
			{
				while( ! guard_.try_lock() )
				{
					wtime::sleep( 0.001 );
				}
				const bool stopped = ready_ <= 0;
				guard_.unlock();
				if( stopped )
					break;
				start_.broadcast();
			}
			
			//-- clean up
			std::cerr << "crew: terminate.wait+quit" << std::endl;			
			while( count_ > 0 )
			{
				worker &agent = agent_[--count_];
				agent.thread_.join();
				destruct( &agent.thread_ );
			}
			memory::kind<memory::global>::release_as<worker>( agent_, (size_t&)size );
		}
		
		void crew:: run() throw()
		{
			check_ready();         //-- recover from last run
			guard_.lock();         //-- wokers won't start directly
			start_.broadcast();    //-- after that call
			//std::cerr << "waiting 1 second1 1/2!" << std::endl;
			//timing::watch::sleep( 1 );
			cycle_.wait( guard_ ); //-- but should after this one
			//std::cerr << "waiting 1 second 2/2!" << std::endl;
			//timing::watch::sleep( 1 );
			guard_.unlock();
			//check_ready();
		}
		
		crew::worker & crew:: operator[]( size_t r ) throw()
		{
			assert( r < size );
			return agent_[r];
		}
		
		const crew::worker & crew:: operator[]( size_t r ) const throw()
		{
			assert( r < size );
			return agent_[r];
		}
		
		void crew:: set_all_args( void *args ) throw()
		{
			for( size_t i=0; i < size; ++i )
			{
				agent_[i].args = args;
			}
		}
		
		
	}
	
}

