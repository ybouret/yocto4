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
			
			void save( array2D &target, const layout2D &sub) const throw();
			inline 
			void save( array2D &target) const throw() { save( target, target ); }
			
			void load( const array2D &source, const layout2D &sub ) throw();
			inline
			void load( const array2D &source ) throw() { load( source, source); }
			
			//! save a raw ppm
			void ppm(const string &        filename, 
					 const string &        comment, 
					 double (*vproc)( const T & ),
					 const color::rgba32 *colors = NULL,
					 double               vmin   = 0,
					 double               vmax   = 1
					 ) const;
			
		private:
			size_t    rows;
			row_type *row;
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
		};
	}
}

#endif
