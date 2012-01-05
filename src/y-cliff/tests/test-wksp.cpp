#include "yocto/utest/run.hpp"


#include "yocto/cliff/wksp1d.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/cliff/wksp3d.hpp"

#include "yocto/math/complex.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace cliff;
using namespace math;
using namespace geom;

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
	
	template <typename WKSP>
	static inline void display_info( const WKSP &wksp)
	{
		std::cerr << "---- layout:" << std::endl;
		display_l(wksp);
		std::cerr << "---- outline: " << std::endl;
		display_l(wksp.outline);
		std::cerr << "---- components: " << std::endl;
		display_c(wksp);
		std::cerr << "min   = " << wksp.region.min << std::endl;
		std::cerr << "max   = " << wksp.region.max << std::endl;
		std::cerr << "delta = " << wksp.delta << std::endl;
		std::cerr << "sizeof(wksp::data_block)=" << sizeof( typename WKSP::data_block) << std::endl;
		std::cerr << "sizeof(wksp::axis_type) =" << sizeof( typename WKSP::axis_type)  << std::endl;
		std::cerr << std::endl;
	}
	
}

YOCTO_UNIT_TEST_IMPL(wksp)
{
	{	
		wksp1D< complex<float>, double>  w1( -10, 20, 
											1, 2, 
											-1.0, 2.0,
											1,2,NULL
											);
		
		display_info( w1 );
		std::cerr << "X=" << w1.X() << std::endl;
	}
	
	{
		const char  *varnames[] = { "u", "v", "w" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		wksp2D< double, float > w2( coord2D(-10,-10), coord2D(20,20),
								   coord2D(0,1), coord2D(0,2),
								   v2d<float>(-1,-1), v2d<float>(2,2),
								   1,varcount, varnames );
		display_info( w2 );
		std::cerr << "X=" << w2.X() << std::endl;
		std::cerr << "Y=" << w2.Y() << std::endl;
	}
	
	{
		const char  *varnames[] = { "A", "B", "C", "D" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		wksp3D< complex<double>, float > w3( coord3D(-10,-10,-10), coord3D(20,20,20),
											coord3D(0,0,1), coord3D(0,0,2),
											v3d<float>(-1,-1,-1), v3d<float>(2,2,2),
											0,varcount, varnames );
		display_info( w3 );
		std::cerr << "X=" << w3.X() << std::endl;
		std::cerr << "Y=" << w3.Y() << std::endl;
		std::cerr << "Z=" << w3.Z() << std::endl;

	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
