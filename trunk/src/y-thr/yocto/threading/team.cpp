#include "yocto/threading/team.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/hw.hpp"

#include "yocto/string/env.hpp"
#include "yocto/exception.hpp"

#include <iostream>
#include <cstdlib>

namespace yocto
{
	namespace threading
	{
		
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
		// layout
		//
		//======================================================================
		static inline bool __next_field( char * &field )
		{
			assert(field!=NULL);
			while( field[0] != '\0' )
			{
				if( !isspace( field[0] ) )
				{
					if( ',' == field[0] )
					{
						++field;
						return true;
					}
					else 
					{
						throw exception("Invalid char '%c' in value", field[0]);
					}

				}
				++field;
			}
			return false;
		}
		
		layout:: layout( ) :
		size( hardware::nprocs() ),
		root(0)
		{
			const string name = "YOCTO_THREADING";
			string       value;
			if( environment::get( value, name ) )
			{
				std::cerr << "[layout parsing '" << value << "']" << std::endl;
				//--------------------------------------------------------------
				// #nproc
				//--------------------------------------------------------------
				const char *text  = value.c_str();
				char       *field = NULL;
				const long  nproc = strtol(text, &field, 10 );
				if( nproc <= 0 )
					throw exception("Invalid number of CPU");
				(size_t&)size = size_t(nproc);
				//--------------------------------------------------------------
				// offset, optional
				//--------------------------------------------------------------
				if( __next_field(field) )
				{
					text = field;
					const long ibase = strtol(text, &field, 10 );
					if( ibase < 0 )
						throw exception("Invalid base CPU");
					(size_t&)root = size_t(ibase);
				}
				
			}
			std::cerr << "[layout=" << size << "," << root << "]" << std::endl;
		}
		
		layout:: layout( const layout &other ) throw() :
		size( other.size ),
		root( other.root )
		{
		}
		
		layout:: ~layout() throw()
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
		
		
		team:: team() :
		layout(),
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
								
				place();
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

