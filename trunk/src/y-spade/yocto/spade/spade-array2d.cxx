#include "yocto/math/ztype.hpp"
#include "yocto/spade/array2d.hpp"

#include "yocto/code/round.hpp"

#include "yocto/geom/color.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/code/utils.hpp"
#include <cstring>

namespace yocto
{
	using namespace math;
	
	namespace spade
	{
		
		template <>
		array2D<z_type>:: ~array2D() throw()
		{
			
		}
		
		template <>
		array2D<z_type>:: array2D( const layout2D &L ) throw() :
		linear_type( L ),
		row_( NULL ),
		row_layout( L.lower.x, L.upper.x ),
		row_bytes( row_layout.items * sizeof(z_type ) )
		{
			assert( row_layout.width == width.x );
		}
		
		
		template <>
		array2D<z_type>::row_type & array2D<z_type>:: operator[]( unit_t y ) throw()
		{
			assert( entry != NULL );
			assert( row_ != NULL );
			assert( y >= lower.y );
			assert( y <= upper.y );
			return row_[ y ];
		}
		
		template <>
		const array2D<z_type>:: row_type & array2D<z_type>:: operator[]( unit_t y ) const throw()
		{
			assert( entry != NULL );
			assert( row_  != NULL );
			assert( y >= lower.y );
			assert( y <= upper.y );
			return row_[ y ];
		}
		
		
		template <>
		array2D<z_type>::type & array2D<z_type>:: operator[]( const coord2D &p ) throw()
		{
			assert( this->has(p) );
			assert( row_   != NULL );
			assert( entry  != NULL );
			return row_[p.y][p.x];
		}
		
		template <>
		array2D<z_type>::const_type & array2D<z_type>:: operator[]( const coord2D &p ) const throw()
		{
			assert( this->has(p) );
			assert( row_   != NULL );
			assert( entry != NULL );
			return row_[p.y][p.x];
		}
		
		
		template <>
		void array2D<z_type>:: copy_into( array2D &sub ) const throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			
			const z_type *src = &row_[ sub.lower.y ][ sub.lower.x ];
			z_type       *dst = sub.entry;
			
			const size_t src_stride = row_layout.items;
			const size_t dst_stride = sub.row_layout.items;
			const size_t scan_line  = sub.row_bytes;
			
			for( size_t y=sub.width.y; y>0; --y, src += src_stride, dst += dst_stride )
			{
				memcpy( dst, src, scan_line );
			}
			
		}
		
		template <>
		void array2D<z_type>:: fill_with( const array2D &sub ) throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			
			z_type       *dst = &row_[ sub.lower.y ][ sub.lower.x ];
			const z_type *src = sub.entry;
			
			const size_t dst_stride = row_layout.items;
			const size_t src_stride = sub.row_layout.items;
			const size_t scan_line  = sub.row_bytes;
			
			for( size_t y=sub.width.y; y>0; --y, src += src_stride, dst += dst_stride )
			{
				memcpy( dst, src, scan_line );
			}
			
		}
		
		template <>
		void array2D<z_type>:: ppm( const string &filename,
								   const string &comment, 
								   double (*vproc)( const z_type & ),
								   const color::rgba32 *colors,
								   double               vmin,
								   double               vmax) const
		{
			assert( vproc != NULL );
			ios::ocstream fp( filename,false );
			fp("P6\n");
			
			//-- comment
			fp("#%s\n", &comment[0] );
			
			//-- size
			fp("%u %u\n", unsigned(width.x), unsigned(width.y) );
			
			//-- #colors
			fp("255\n");
			
			
			//-- data
			//-- #info
			const z_type *p = entry;
			if( NULL == colors )
			{
				for( size_t i=items; i>0; --i, ++p)
				{
					const color::rgba<double> c = color::rgba<double>::ramp( vproc(*p), vmin, vmax );
					const uint8_t b[4] =  { uint8_t( 255 * c.r ), uint8_t( 255 * c.g ), uint8_t(255 * c.b ), 255 };
					fp.write( b[0] );
					fp.write( b[1] );
					fp.write( b[2] );
				}
			}
			else 
			{
				for( size_t i=items; i>0; --i, ++p)
				{
					const color::rgba32 c = color::rgba32:: ramp( colors,
																 vproc(*p),
																 vmin, 
																 vmax );
					fp.write( c.r );
					fp.write( c.g );
					fp.write( c.b );
				}
			}
			
			
		}
		
		
		
		template <>
		void array2D<z_type>:: request( size_t lengths[] ) const throw()
		{
			assert( lengths );
			
			const size_t mem_for_rows = width.y * sizeof( row_type );
			lengths[0] = YOCTO_MEMALIGN( mem_for_rows );
			lengths[1] = bytes;
			
			assert( bytes == width.y * row_bytes );
		}
		
		template <>
		void array2D<z_type>:: link_to( void * buffers[]) throw()
		{
			assert( buffers );
			assert( buffers[0] );
			assert( buffers[1] );
			assert( size_t((char *)buffers[1] - (char*)buffers[0]) >= width.y * sizeof(row_type)  );
			
			assert( NULL == entry );
			assert( NULL == row_  );
			
			row_  = (row_type *) buffers[0];
			row_ -= lower.y;
			entry = (z_type *) buffers[1];
			
			void * row_buffers[] = { entry };
			
			for( unit_t y = lower.y; y <= upper.y; ++y )
			{
				row_type *r = new (& row_[y]) row_type( row_layout );
				r->link_to( row_buffers  );
				update( row_buffers[0], row_bytes );
			}
			
#			if !defined(NDEBUG)
			for( unit_t y=lower.y; y < upper.y; ++y )
			{
				const z_type *curr = row_[y].entry;
				const z_type *next = row_[y+1].entry;
				assert( size_t(next-curr) == row_layout.items );
			}
#			endif
			
			
		}
		
#if 0
		template <>
		void array2D<z_type>:: ppm(const string &filename, 
								   const string &comment,
								   double       (*vproc)( const z_type & ),
								   double               Lx,
								   double               Ly,
								   size_t               ppu,
								   const color::rgba32 *colors,
								   double               vmin,
								   double               vmax
								   ) const
		{
			
			assert( vproc != NULL );
			assert(Lx>0);
			assert(Ly>0);

			ios::ocstream fp( filename );
					
			fp("P6\n");
			
			//-- comment
			fp("#%s\n", &comment[0] );
			
			ppu = max_of<size_t>(ppu,2);
			
			
			
			
		}
#endif
		
		
		
	}
	
}

