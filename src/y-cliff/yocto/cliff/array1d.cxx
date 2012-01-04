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
			assert( this->has(x) );
			
			return item_[x];
		}
		
		template <>
		const z_type & array1D<z_type>:: operator[]( unit_t x ) const throw()
		{
			assert( entry!=NULL  );
			assert( item_!=NULL  );
			assert( this->has(x) );
			
			return item_[x];
		}
		
		
		template <>
		void array1D<z_type>:: link( z_type *addr ) throw()
		{
			assert(NULL!=addr);
			entry = addr;
			item_ = addr-lower;
		}
		
	}
}
