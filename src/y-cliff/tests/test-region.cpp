#include "yocto/utest/run.hpp"


#include "yocto/cliff/array3d.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	
	template <typename U>
	static inline void gen_vtx( void *p, size_t n ) throw()
	{
		assert(p);
		U *q = (U*)p;
		for( size_t i=0; i < n; ++i )
			q[i] = 10 * (0.5-alea<U>());
	}
	
	template <typename U,template <class> class REGION>
	static inline void perform()
	{
		typedef typename REGION<U>::type     region_type;
		typedef typename region_type::vertex vertex;
		std::cerr << "-- Region " << region_type::DIMENSIONS << "D" << std::endl;
		
		vertex u,v;
		gen_vtx<U>( &u, region_type::DIMENSIONS );
		gen_vtx<U>( &v, region_type::DIMENSIONS );
		region_type R(u,v);
		std::cerr << "-- min   = " << R.min    << std::endl;
		std::cerr << "-- max   = " << R.max    << std::endl;
		std::cerr << "-- length= " << R.length << std::endl;
		std::cerr << "-- space = " << R.space  << std::endl;
		
	}

}

YOCTO_UNIT_TEST_IMPL(region)
{
	
	perform<float,region1D>();
	perform<double,region1D>();
	
	perform<float,region2D>();
	perform<double,region2D>();
	
	perform<float,region3D>();
	perform<double,region3D>();
	
}
YOCTO_UNIT_TEST_DONE()
