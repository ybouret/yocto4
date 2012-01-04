#include "yocto/cliff/array2d.hpp"
#include "yocto/math/ztype.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
	namespace cliff
	{
		using math::z_type;
		
		template <>
		array2D<z_type>:: array2D( const layout2D &L) :
		linear_type(L),
		row_layout( lower.x, upper.x ),
		rows( width.y ),
		row( memory::kind<memory::global>::acquire_as<row_type>(rows) - lower.y )
		{
			for( unit_t y = lower.y; y <= upper.y; ++y )
				new ( &row[y] ) row_type( row_layout );
			
		}
		
		
		template <>
		array2D<z_type>:: ~array2D() throw()
		{
			for( unit_t y = upper.y; y >= lower.y; --y)
			{
				row[y].~row_type();
			}
			row += lower.y;
			memory::kind<memory::global>::release_as<row_type>(row,rows) ;
		}
		
		
		template <>
		void array2D<z_type>:: link( z_type *addr ) throw()
		{
			entry = addr;
			for( unit_t y = lower.y; y <= upper.y; ++y, addr += row_layout.items )
				row[y].link(addr);
		}
		
		template<>
		array2D<z_type>::row_type & array2D<z_type>:: operator[]( unit_t y ) throw()
		{
			assert( entry != NULL );
			assert( y >= lower.y );
			assert( y <= upper.y );
			return row[y];
		}
		
		template<>
		const array2D<z_type>::row_type & array2D<z_type>:: operator[]( unit_t y ) const throw()
		{
			assert( entry != NULL );
			assert( y >= lower.y );
			assert( y <= upper.y );
			return row[y];
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
		void array2D<z_type>:: save( array2D<z_type> &target, const layout2D &sub) const throw()
		{
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert( target.has( sub.lower ) );
			assert( target.has( sub.upper ) );
			
			const array2D<z_type> &self = *this;
			for( unit_t y=sub.upper.y; y >= sub.lower.y; --y )
			{
				const row_type &r_src = self[y];
				row_type       &r_tgt = target[y];
				for( unit_t x=sub.upper.x; x >= sub.lower.x; --x )
				{
					r_tgt[x] = r_src[x];
				}
			}
			
		}
		
		template <>
		void array2D<z_type>:: load( const array2D<z_type> &source, const layout2D &sub) throw()
		{
			
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert( source.has( sub.lower ) );
			assert( source.has( sub.upper ) );
			
			array2D<z_type> &self = *this;
			for( unit_t y=sub.upper.y; y >= sub.lower.y; --y )
			{
				const row_type &r_src = source[y];
				row_type       &r_tgt = self[y];
				for( unit_t x=sub.upper.x; x >= sub.lower.x; --x )
				{
					r_tgt[x] = r_src[x];
				}
			}
		}
		
		
		
		
	}
	
}


