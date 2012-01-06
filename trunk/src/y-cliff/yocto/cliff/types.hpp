//! \file
/**
 Cartesian Lightweight Implementation of Fields Fragmentation
 */
#ifndef YOCTO_CLIFF_TYPES_INCLUDED
#define YOCTO_CLIFF_TYPES_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		typedef ptrdiff_t unit_t;
		
		class layout_base
		{
		public:
			const size_t dimensions;
			virtual ~layout_base() throw();
			
			//! MPI style splitting
			static void split(unit_t      &lo, 
							  unit_t      &hi, 
							  const unit_t Lo, 
							  const unit_t Hi, 
							  const size_t rank, 
							  const size_t size );
			
		protected:
			explicit layout_base(const size_t ndim) throw();
			//! order coordinates, compute width and return #items
			size_t   setup( const void *lo, const void *up, const void *w );
			void     inside( unit_t *lo, unit_t *up, const unit_t *w ) const;
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(layout_base);
		};
		
		
	}
	
}

#endif
