#include "yocto/utest/run.hpp"

using namespace yocto;

#include "yocto/spade/layout.hpp"
#include "yocto/spade/in1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/spade/in3d.hpp"

#include "yocto/spade/linear.hpp"
#include "yocto/memory/global.hpp"

namespace {
	
	typedef float Real;
	
	template <typename LAYOUT>
	void process( const LAYOUT &L )
	{
		for( size_t idim =0; idim < L.dims ; ++idim )
		{
			std::cerr << "Layout: " << L.lower << " -> " << L.upper << std::endl;
			std::cerr << "\twidth=" << L.width << std::endl;
			std::cerr << "\titems=" << L.items << std::endl;
			
			for( size_t np=1; np <=4; ++np )
			{
				std::cerr << "Cut in " << np << " along coord #" << idim << std::endl;
				for( size_t rank = 0 ; rank < np; ++rank )
				{
					const LAYOUT  ls = L.split( np, rank, idim );
					std::cerr << "\t\t" << rank << ":" << ls.lower << " -> " << ls.upper << " => " << ls.width << ", #=" << ls.items << std::endl;
				}
			}
		}
		
	}
}

YOCTO_UNIT_TEST_IMPL(layouts)
{
	std::cerr << "Testing Layout Splitting" << std::endl;
	
	spade::layout1D L1(10,100 );
	spade::layout2D L2( spade::coord2D(15,-5),   spade::coord2D(-5,25)  );
	spade::layout3D L3( spade::coord3D(15,-5,7), spade::coord3D(-6,25,-2) );
	
	process( L1 );
	process( L2 );
	process( L3 );
	
	
	
}
YOCTO_UNIT_TEST_DONE()

