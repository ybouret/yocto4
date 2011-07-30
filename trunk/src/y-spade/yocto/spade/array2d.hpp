#ifndef YOCTO_SPADE_ARRAY2D_INCLUDED
#define YOCTO_SPADE_ARRAY2D_INCLUDED 1

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/string.hpp"
#include "yocto/geom/color.hpp"


namespace yocto
{
	
	namespace spade
	{
		
		
		//! y rows
		/**
		 the memory is allocated for width.y rows
		 and for rows * width.x items
		 */
		
		template <typename T>
		class array2D : public linear<T,layout2D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef layout2D           layout_type;
			typedef linear<T,layout2D> linear_type;			
			typedef array1D<T>          row_type;
			
			explicit array2D( const layout2D &L ) throw();
			virtual ~array2D() throw();
			
			row_type       & operator[]( unit_t y ) throw();
			const row_type & operator[]( unit_t y ) const throw();
			
			type           & operator[]( const coord2D &p ) throw();
			const_type     & operator[]( const coord2D &p ) const throw();
			
			
			
			
			void copy_into( array2D &sub ) const throw();
			void fill_with( const array2D &sub ) throw();
			
			//! save a raw ppm
			void ppm(const string &        filename, 
					 const string &        comment, 
					 double (*vproc)( const T & ),
					 const color::rgba32 *colors = NULL,
					 double               vmin   = 0,
					 double               vmax   = 1
					 ) const;
			
						
			//! compute lengths
			/**
			 \param lengths
			 - lengths[0] -> memory for rows = width.y * sizeof( row_type )
			 - lengths[1] -> memory for data = width.y * row_bytes
			 */
			virtual void request( size_t lengths[] ) const throw();
			
			//! build rows
			/**
			 \param buffers
			 - buffers[0]: memory for width.y rows
			 - buffers[1]: memory for all data
			 */
			virtual void link_to( void * buffers[] ) throw();
			
#if 0
			inline unit_t offset_of( unit_t x, unit_t y ) const throw()
			{ 
				const unit_t dx = x - this->lower.x;
				const unit_t dy = y - this->lower.y;
				const unit_t wx = this->width.x;
				return dx + dy * wx;
			}
#endif
			
		private:
			row_type       *row_; //!< offseted row, NULL at start
			YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
			
		public:
			const layout1D  row_layout;  //!< lower.x -> upper.x
			const size_t    row_bytes;   //!< row_layout.items * sizeof(T)
		};
		
	}
	
}


#endif
