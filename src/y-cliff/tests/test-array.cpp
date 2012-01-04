#include "yocto/utest/run.hpp"

using namespace yocto;

#include "yocto/cliff/layout.hpp"
#include "yocto/cliff/array2d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/math/complex.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	static inline void
	gen_coord( void *p, size_t n ) throw()
	{
		unit_t *u = (unit_t *)p;
		for( size_t i=0; i < n; ++i )
		{
			u[i] = unit_t( 40 * (0.5 - alea<double>()));
		}
	}
	
	template<typename LAYOUT>
	static inline void display( const LAYOUT &L )
	{
		std::cerr << "-- " << LAYOUT::DIMENSIONS << "D" << std::endl;
		std::cerr << "-- lower= " << L.lower << std::endl;
		std::cerr << "-- upper= " << L.upper << std::endl;
		std::cerr << "-- width= " << L.width << std::endl;
	}
	
	template <typename T>
	void perform1D()
	{
		coord1D U,V;
		gen_coord(&U, 1);
		gen_coord(&V, 1);
		const layout1D L(U,V);
		display(L);
		array1D<T>     A(L);
	}

	template <typename T>
	void perform2D()
	{
		coord2D U,V;
		gen_coord(&U, 2);
		gen_coord(&V, 2);
		const layout2D L(U,V);
		display(L);
		array2D<T>     A(L);
	}
	
	
}


YOCTO_UNIT_TEST_IMPL(array)
{
	perform1D<float>();
	perform1D<double>();
	perform1D< math::complex<float> >();
	perform1D< math::complex<double> >();
	
	perform2D<float>();
	perform2D<double>();
	perform2D< math::complex<float> >();
	perform2D< math::complex<double> >();
	

}
YOCTO_UNIT_TEST_DONE()
