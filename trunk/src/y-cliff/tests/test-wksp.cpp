#include "yocto/utest/run.hpp"


#include "yocto/cliff/array3d.hpp"
#include "yocto/cliff/workspace.hpp"

#include "yocto/code/rand.hpp"
using namespace yocto;
using namespace cliff;
using namespace math;


namespace {
	
	
	template<typename LAYOUT>
	static inline void display_l( const LAYOUT &L )
	{
		std::cerr << "-- " << LAYOUT::DIMENSIONS << "D" << std::endl;
		std::cerr << "-- lower= " << L.lower << std::endl;
		std::cerr << "-- upper= " << L.upper << std::endl;
		std::cerr << "-- width= " << L.width << std::endl;
		std::cerr << "-- items= " << L.items << std::endl;
	}
	
	static inline void display_c( const components &C )
	{
		for( size_t i=C.cmin; i <= C.cmax; ++i )
		{
			std::cerr << "component #" << i << " = " << C.get_name(i) << std::endl;
		}
	}
	
	
}

YOCTO_UNIT_TEST_IMPL(wksp)
{
	
	typedef workspace< complex<float>, array1D, double, region1D > w1_t;
	
	w1_t w1( -50, 100, 
			1, 2, 
			-1.0, 2.0,
			1,2,NULL
			);
	
	std::cerr << "layout:" << std::endl;
	display_l(w1);
	std::cerr << "outline: " << std::endl;
	display_l(w1.outline);
	std::cerr << "components: " << std::endl;
	display_c(w1);
	std::cerr << "min   = " << w1.region.min << std::endl;
	std::cerr << "max   = " << w1.region.max << std::endl;
	std::cerr << "delta = " << w1.delta << std::endl;
	
	std::cerr << "sizeof(workspace::data_block): " << sizeof(w1_t::data_block) << std::endl;
	std::cerr << "sizeof(workspace::axis_type) : " << sizeof(w1_t::axis_type)  << std::endl;
	const array1D<double> &X = w1.axis(0);
	std::cerr << "X=" << X << std::endl;

}
YOCTO_UNIT_TEST_DONE()
