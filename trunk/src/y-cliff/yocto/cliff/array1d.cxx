#include "yocto/cliff/array1d.hpp"
#include "yocto/math/ztype.hpp"


namespace yocto
{
	namespace cliff
	{
		using math::z_type;
		
		template <>
		array1D<z_type>:: array1D( const layout1D &L) throw() :
		linear_type(L),
		item_(NULL)
		{
		}
		
		
		template <>
		array1D<z_type>:: ~array1D() throw()
		{
		}
		
		
		template <>
		z_type & array1D<z_type>:: operator[]( unit_t x ) throw()
		{
			assert( entry!=NULL  );
			assert( item_!=NULL  );
			assert( x >= lower );
			assert( x <= upper );
			
			return item_[x];
		}
		
		template <>
		const z_type & array1D<z_type>:: operator[]( unit_t x ) const throw()
		{
			assert( entry!=NULL  );
			assert( item_!=NULL  );
			assert( x >= lower );
			assert( x <= upper );
			
			return item_[x];
		}
		
		
		template <>
		void array1D<z_type>:: link( z_type *addr ) throw()
		{
			assert(NULL!=addr);
			entry = addr;
			item_ = addr-lower;
		}
		
		
		template <>
		void array1D<z_type>:: save( array1D<z_type> &target, const layout1D &sub ) const throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			assert( target.has( sub.lower ) );
			assert( target.has( sub.upper ) );
			
			const array1D<z_type> &self = *this;
			z_type                *q    = &target[sub.lower];
			const z_type          *p    = &self[sub.lower];
			
			for( size_t i=sub.items;i>0;--i)
			{
				*(q++) = *(p++);
			}
		}
		
		
		template <>
		void array1D<z_type>:: load( const array1D<z_type> &source, const layout1D &sub ) throw()
		{
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert( source.has( sub.lower ) );
			assert( source.has( sub.upper ) );
			
			array1D<z_type> &self = *this;
			z_type          *q    = &self[sub.lower];
			const z_type    *p    = &source[sub.lower];
			
			for( size_t i=sub.items;i>0;--i)
			{
				*(q++) = *(p++);
			}
			
		}
		
	}
}
