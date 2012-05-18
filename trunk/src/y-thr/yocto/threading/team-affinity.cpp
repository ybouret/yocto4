#include "yocto/threading/team.hpp"
#include "yocto/threading/thread.hpp"

#include "yocto/exceptions.hpp"
#include "yocto/hw.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		
		
#include "./team-member.hxx"
		
		static inline size_t cpu_index(const size_t        i,
									   const team::layout &l,
									   const size_t        n ) throw()
		{
			const size_t j =  ( l.root + i % l.size ) % n;
			std::cerr << " |_ assign on CPU #" << j << "/" << n << std::endl;
			return j;
		}
		
		void team:: place(  )
		{
			assert( size > 0 );
			const size_t  cpu_setsize = hardware::nprocs();
			member       *m           = static_cast<member *>(wksp_);
			//------------------------------------------------------------------
			// assign main thread
			//------------------------------------------------------------------
			std::cerr << "-- main thread: " << std::endl;
            thread::assign_cpu( thread::get_current_handle(), cpu_index(0,*this,cpu_setsize) );
			//__assign( thread::get_current_handle(), cpu_index( 0, *this, cpu_setsize ) );
			
			//------------------------------------------------------------------
			// assign team thread
			//------------------------------------------------------------------
			std::cerr << "-- members    : " << std::endl;
			for( size_t i=0; i < size; ++i )
            {
                thread::assign_cpu( m[i].thr.get_handle(), cpu_index(i,*this,cpu_setsize) );
            }
            //__assign(  m[i].thr.get_handle(), cpu_index( i, *this, cpu_setsize ) );
			
			
		}
        
		
	}
}
