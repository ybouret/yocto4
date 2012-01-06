#include "yocto/cliff/array1d.hpp"
#include "yocto/math/ztype.hpp"

#include <iostream>

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
		void array1D<z_type>:: foreach( const layout1D &sub, callback_type proc, void *args )
		{
			assert(proc);
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			z_type          *p = & (*this)[sub.lower];
			for( size_t i=sub.items;i>0;--i)
			{
				proc(*(p++),args);
			}
		}
		

		template <>
		void array1D<z_type>:: foreach( const layout1D &sub, const_cb_type proc, void *args ) const
		{
			assert(proc);
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			const z_type *p = & (*this)[sub.lower];
			for( size_t i=sub.items;i>0;--i)
			{
				proc(*(p++),args);
			}
		}
		
		
		template <>
		void array1D<z_type>:: foreach( const array1D<z_type> &other, const layout1D &sub, callback2_type proc, void *args )
		{
			assert(proc);
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert(  other.has( sub.lower ) );
			assert(  other.has( sub.upper ) );
			
			z_type          *p = & (*this)[sub.lower];
			const z_type    *q = &   other[sub.lower];
			for( size_t i=sub.items;i>0;--i)
			{
				proc(*(p++),*(q++),args);
			}
		}
		
		
		
		
		template <>
		std::ostream & operator<<( std::ostream &os, const array1D<z_type> &a )
		{
			os << '[';
			for( unit_t x=a.lower; x <= a.upper; ++x )
			{
				os << ' ' << a[x];
			}
			os << ' ' << ']';
			return os;
		}
		
				
	}
}
