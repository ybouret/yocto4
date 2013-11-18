#ifndef YOCTO_MEMORY_CARVER_INCLUDED
#define YOCTO_MEMORY_CARVER_INCLUDED 1


#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
	namespace memory 
	{
		class slice;
		
		class carver 
		{
		public:
			explicit carver( size_t mini, size_t unused_max) throw();
			virtual ~carver() throw();
			
			
			
			void  *acquire( size_t &bytes );
			void   release( void *p ) throw();
			
			size_t used_slices() const throw();
			size_t pool_slices() const throw();
			
		private:
			core::list_of<slice> list_;
			core::pool_of<slice> pool_;
			slice               *free_; //!< a current free page
			YOCTO_DISABLE_COPY_AND_ASSIGN(carver);
		public:
			size_t mini_size;
			size_t max_in_pool;
		};
		
		
	}
}

#endif
