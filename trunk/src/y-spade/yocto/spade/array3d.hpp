#ifndef YOCTO_SPADE_ARRAY3D_INCLUDED
#define YOCTO_SPADE_ARRAY3D_INCLUDED 1

#include "yocto/spade/array2d.hpp"
#include "yocto/spade/in3d.hpp"


namespace yocto
{
	
	namespace spade
	{
		
		template <typename T>
		class array3D : public linear<T,layout3D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			typedef layout3D                      layout_type;
			typedef linear<T,layout3D>            linear_type;			
			typedef array2D<T>                    slice_type;
			typedef typename slice_type::row_type row_type;
			
			explicit array3D( const layout3D &L ) throw();			
			virtual ~array3D() throw() {}
			
			slice_type &       operator[]( unit_t y ) throw();
			const slice_type & operator[]( unit_t y ) const throw();
			
			type &       operator[]( const coord3D & p ) throw();
			const_type & operator[]( const coord3D & p ) const throw();
			
			//! compute lengths
			/**
			 \param lengths
			 - lengths[0] memory for slices     = width.z * sizeof(slice_type)
			 - lengths[1] memory for slices_row = width.z * width.y * sizeof(row_type)
			 - lengths[2] memory for items      = bytes
			 */
			virtual void request( size_t lengths[] ) const throw();
			
			//! set buffers
			virtual void link_to( void  *buffers[] ) throw();
			
			
			void copy_into( array3D &sub ) const throw();
			void fill_with( const array3D &sub ) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
			slice_type *slice_;
			
		public:
			const layout2D  slice_layout;  //!< layout of one slice
			const size_t    slice_bytes;   //!< length of one slice in bytes = slice_layout.items * slice_bytes
			
			
		};
		
	}
	
	
}

#endif


