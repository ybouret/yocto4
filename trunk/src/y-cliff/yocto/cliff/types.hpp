//! \file
/**
 Cartesian Lightweight Implementation of Fields Fragmentation
 */
#ifndef YOCTO_CLIFF_TYPES_INCLUDED
#define YOCTO_CLIFF_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
	
	namespace cliff
	{


		
		//! signed integer type for indexing
		typedef ptrdiff_t unit_t;
		
		//! base class for layouts
		class layout_base
		{
		public:
			const size_t dimensions; //!< set at initial time
			virtual ~layout_base() throw();
			
			//! MPI style splitting for one dimension
			/**
			 \param lo final lower coordinate
			 \param hi final upper coordinate
			 \param Lo source lower coordinate
			 \param Hi source upper coordinate
			 \param rank MPI style rank, 0 <= rank < size
			 \param size MPI style size
			 */
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
			
			//! compute strictly inside coordinates
			void     inside( unit_t *lo, unit_t *up, const unit_t *w ) const;
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(layout_base);
		};
		
		typedef sorted_vector<size_t> offsets_list; //!< database of offsets

		
	}
	
}

#endif
