#ifndef YOCTO_SPADE_TYPES_INCLUDED
#define YOCTO_SPADE_TYPES_INCLUDED 1

#include "yocto/object.hpp"

namespace yocto
{

	namespace spade
	{

		typedef int unit_t; //!< base signed integer

		class layout_base : public object
		{
		public:
			virtual ~layout_base() throw();


			//! order coordinates and count items
			/**
			 \param umin address of the x min coordinate (MODIFIED)
			 \param umax address of the x max coordinate (MODIFIED)
			 \param ndim number of dimensions
			 \param ulen address of the x length coordinate (MODIFIED)
			 \return the number of items
			 */
			static size_t order(const void *umin,
								const void *umax,
								size_t      ndim,
								const void *ulen) throw();

			//! count the items
			/**
			 \param  ulen address of the x length coordinate (UNTOUCHED)
			 \return the product of coordinates
			 */
			static size_t count(const void *ulen,
								size_t ndim) throw();


			static void   split1(unit_t &L,
								 unit_t &R,
								 size_t  world_size,
								 size_t  world_rank ) throw();

			//! increase buffers address
			static void update( void * &buffer, const size_t length ) throw();
			
			//! lengths->offsets, returns total bytes
			static size_t len2off( size_t lengths[], size_t ndim ) throw();
			
		protected:
			explicit layout_base() throw();



		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN( layout_base );
		};



	}


}

#endif

