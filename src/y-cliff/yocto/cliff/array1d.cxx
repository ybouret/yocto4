#include "yocto/cliff/array1d.hpp"
#include "yocto/math/ztype.hpp"


namespace yocto
{
	namespace cliff
	{
		using math::z_type;
		
		template <>
		array1D<z_type>:: array1D( const layout1D &L) throw() :
		linear_type(L)
		{
		}
		
		
		template <>
		array1D<z_type>:: ~array1D() throw()
		{
		}
		
		
		template <>
		z_type & array1D<z_type>:: operator[]( unit_t x ) throw()
		{
			
		}
		
	}
}
