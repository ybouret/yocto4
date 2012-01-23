//! \file

#ifndef YOCTO_CLIFF_ARRAY2D_INCLUDED
#define YOCTO_CLIFF_ARRAY2D_INCLUDED 1

#include "yocto/cliff/array1d.hpp"
#include "yocto/cliff/in2d.hpp"

#include "yocto/geom/color.hpp"
#include "yocto/string.hpp"

namespace yocto
{
	
	namespace cliff
	{	
		
		//! two dimensions array of float/double/complex...
		template <typename T>
		class array2D : public linear<T,layout2D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef linear<T,layout2D> linear_type;
			typedef layout2D           layout_type;
			typedef array1D<T>         row_type;
			
			explicit array2D( const layout_type &L );
			virtual ~array2D() throw();
			virtual void link( T *addr ) throw();
			
			const layout1D row_layout;
			
			row_type       & operator[]( unit_t y ) throw();
			const row_type & operator[]( unit_t y ) const throw();
			
			typedef typename linear_type::callback_type  callback_type;
			typedef typename linear_type::const_cb_type  const_cb_type;
			typedef typename linear_type::callback2_type callback2_type;			
			virtual void foreach( const layout2D &sub, callback_type  proc, void *args);
			virtual void foreach( const layout2D &sub, const_cb_type  proc, void *args) const;
			void         foreach( const array2D<T> &other, const layout2D &sub, callback2_type proc, void *args);
			
			//! save a raw ppm
			void ppm(const string        &filename, 
					 const string        &comment,
					 const layout2D      &area,
					 double             (*vproc)( const T & ),
					 const color::rgba32 *colors = NULL,
					 double               vmin   = 0,
					 double               vmax   = 1
					 ) const;
			
			template <typename U>
			friend std::ostream & operator<<( std::ostream &os, const array2D<U> &a);
			
			YOCTO_CLIFF_ARRAY_OPS(array2D)
			
		private:
			size_t    rows;
			row_type *row;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
		};
	}
}

#endif
