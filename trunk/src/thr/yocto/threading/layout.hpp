//! \file
#ifndef YOCTO_THREADING_LAYOUT_INCLUDED
#define YOCTO_THREADING_LAYOUT_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
	
	namespace threading
	{
		
		//! How many threads, from which CPU
		/**
		 Prepare a threading layout from a
		 default configuration or from the YOCTO_THREADING environment
		 variable.
		 */
		class layout
		{
		public:
			const size_t size; //!< #threads requested
			const size_t root; //!< #CPU to start.
			const size_t ncpu; //!< hardware #CPU
            const size_t scan; //!< min_of(size,ncpu-root)
            
			//! parse the environment variable YOCTO_THREADING=#threads[,offset]
			/**
			 - if YOCTO_THREADING is undefined then size = hardware::nprocs(), root=0
			 - if YOCTO_THREADING is defined, then the first field is #threads (mandatory)
			 while the optional second field is the CPU offset.
			 */
			explicit layout(); 
			
            explicit layout( size_t num_threads, size_t thread_offset = 0);
            
			//! destructor.
			virtual ~layout() throw();
			
			//! copy size and root.
			layout( const layout &other ) throw();
			
            //! where to assign thread #i
            size_t cpu_index_of( size_t iThread ) const throw();
            
            
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
	}
	
}

#endif
