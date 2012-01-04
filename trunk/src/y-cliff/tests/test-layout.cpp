#include "yocto/utest/run.hpp"

using namespace yocto;

#include "yocto/cliff/layout.hpp"
#include "yocto/cliff/in1d.hpp"
#include "yocto/cliff/in2d.hpp"
#include "yocto/cliff/in3d.hpp"

#include "yocto/code/rand.hpp"


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
	
	template <typename LAYOUT>
	static inline void perform()
	{
		typename LAYOUT::coord_type U,V;
		gen_coord(&U,LAYOUT::DIMENSIONS);
		gen_coord(&V,LAYOUT::DIMENSIONS);
		
		LAYOUT L(U,V);
		std::cerr << "#dims="  << L.dimensions << "/" << L.DIMENSIONS << std::endl;
		std::cerr << "lower= " << L.lower << std::endl;
		std::cerr << "upper= " << L.upper << std::endl;
		std::cerr << "width= " << L.width << std::endl;
		std::cerr << "items= " << L.items << std::endl;
		
		try
		{
			for( size_t size=1; size <= 8; ++size )
			{
				std::cerr << "-- spliting in " << size << std::endl;
				for( size_t rank=0; rank < size; ++rank )
				{
					const LAYOUT sub( L.split(rank,size) );
					std::cerr << "\t @" << rank << ": " << sub.lower << " -> " << sub.upper << " = " << sub.width << std::endl;
				}
			}
		}
		catch(const exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << e.when() << std::endl;
		}
		
	}
}


YOCTO_UNIT_TEST_IMPL(layout)
{
	perform<layout1D>();
	perform<layout2D>();
	perform<layout3D>();
}
YOCTO_UNIT_TEST_DONE()
