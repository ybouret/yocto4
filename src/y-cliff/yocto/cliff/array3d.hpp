#ifndef YOCTO_CLIFF_ARRAY3D_INCLUDED
#define YOCTO_CLIFF_ARRAY3D_INCLUDED 1

#include "yocto/cliff/array2d.hpp"
#include "yocto/cliff/in3d.hpp"


namespace yocto
{
	
	namespace cliff
	{	
		
		template <typename T>
		class array3D : public linear<T,layout3D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef linear<T,layout3D> linear_type;
			typedef layout3D           layout_type;
			typedef array2D<T>         slice_type;
			
			explicit array3D( const layout_type &L );
			virtual ~array3D() throw();
			virtual void link( T *addr ) throw();
			
			const layout2D slice_layout;
			
			slice_type       & operator[]( unit_t z ) throw();
			const slice_type & operator[]( unit_t z ) const throw();
			
			void copy(   const array3D &source, const layout3D &sub ) throw();
			void add(    const array3D &source, const layout3D &sub ) throw();
			void muladd( param_type k, const array3D &source, const layout3D &sub ) throw();

		private:
			size_t      slices;
			slice_type *slice;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
		};
	}
}

#endif
