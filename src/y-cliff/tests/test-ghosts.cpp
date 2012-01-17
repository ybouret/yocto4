#include "yocto/utest/run.hpp"


#include "yocto/cliff/wksp1d.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/cliff/wksp3d.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
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
			std::cerr << "component #" << i << " = " << C.name(i) << std::endl;
		}
	}
	
}

YOCTO_UNIT_TEST_IMPL(ghosts)
{
	const size_t cmin = 1;
	const char  *names[] = { "u", "v" };
	const size_t cnum = sizeof(names)/sizeof(names[0]);
	
	{
		typedef wksp1D<double,double> w1D_t;
		w1D_t W(-10,10,
				0, 1,
				-1, 1,
				cmin, cnum, names );
		std::cerr << "workspace layout: " << std::endl;
		display_l( W );
		std::cerr << "worksapce outline: " << std::endl;
		display_l( W.outline );
		
	}
	
}
YOCTO_UNIT_TEST_DONE()
