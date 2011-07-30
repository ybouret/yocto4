#ifndef YOCTO_SPADE_ARRAY1D_INCLUDED
#define YOCTO_SPADE_ARRAY1D_INCLUDED 1

#include "yocto/spade/in1d.hpp"
#include "yocto/spade/linear.hpp"

namespace yocto
{

	namespace spade
	{
		//! linear array of items
		/**
			the requested memory is only for the tableau 'entry', with 'bytes' long.
		 */

		template <typename T>
		class array1D : public linear<T,layout1D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;

			typedef linear<T,layout1D> linear_type;
			typedef layout1D           layout_type;
			
			explicit array1D( const layout1D &L ) throw();
			virtual ~array1D() throw();

			type       & operator[]( unit_t x ) throw();
			const_type & operator[]( unit_t x ) const throw();

			
			//! compute the requested size for this array
			/**
				\param  lengths lengths[0] = bytes
				\return lengths[0]
			 */
			virtual void request( size_t length[] ) const throw();
			
			//! perform memory linkage
			/**
				\param buffers buffers[0]  -> entry
			 */
			virtual void link_to( void * buffers[] ) throw();
			
			void copy_into( array1D &sub ) const throw();
			void fill_with( const array1D &sub ) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
			mutable_type *item_; //!< offseted entry
			
		};

		
		
	}

}


#endif
