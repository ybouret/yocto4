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
			
			typedef typename linear_type::callback_type  callback_type;
			typedef typename linear_type::const_cb_type  const_cb_type;
			typedef typename linear_type::callback2_type callback2_type;			
			virtual void foreach( const layout3D &sub, callback_type  proc, void *args);
			virtual void foreach( const layout3D &sub, const_cb_type  proc, void *args) const;
			void         foreach( const array3D<T> &other, const layout3D &sub, callback2_type proc, void *args);
			
			YOCTO_CLIFF_ARRAY_OPS(array3D)
			
		private:
			size_t      slices;
			slice_type *slice;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
		};
	}
}

#endif
