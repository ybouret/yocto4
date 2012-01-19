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
			q[i] = U(10) * (U(0.5)-alea<U>());
	}
	
	static inline void gen_coord( void *p, size_t n, unit_t v ) throw()
	{
		assert(p);
		unit_t *q = (unit_t *)p;
		for( size_t i=0; i < n; ++i )
		{
			q[i] = v;
		}
	}
	
	template <typename REGION>
	static inline void display_r( const REGION &R )
	{
		std::cerr << "-- min   = " << R.min    << std::endl;
		std::cerr << "-- max   = " << R.max    << std::endl;
		std::cerr << "-- length= " << R.length << std::endl;
		std::cerr << "-- space = " << R.space  << std::endl;
	}
	
	template <typename U,template <class> class REGION,class LAYOUT>
	static inline void perform()
	{
		typedef typename REGION<U>::type       region_type;
		typedef typename region_type::vertex_t vertex_t;
		std::cerr << "-- Region " << region_type::DIMENSIONS << "D" << std::endl;
		
		vertex_t u,v;
		gen_vtx<U>( &u, region_type::DIMENSIONS );
		gen_vtx<U>( &v, region_type::DIMENSIONS );
		region_type R(u,v);
		
		std::cerr << "---- Full Region: " << std::endl;
		
		display_r(R);
		
		typedef typename LAYOUT::coord_t coord_t;
		assert( region_type::DIMENSIONS==LAYOUT::DIMENSIONS);
		coord_t a,b,c,d;
		gen_coord( &a, LAYOUT::DIMENSIONS, 1 );
		gen_coord( &b, LAYOUT::DIMENSIONS, 10 );
		
		const LAYOUT master(a,b);
		
		gen_coord( &c, LAYOUT::DIMENSIONS, 3 );
		gen_coord( &d, LAYOUT::DIMENSIONS, 7 );
		
		const LAYOUT sub(c,d);
		
		
		std::cerr << "---- Sub Region: " << std::endl;
		region_type rs( region_type::extract( R, master, sub ) );
		display_r(rs);
	}

}

YOCTO_UNIT_TEST_IMPL(region)
{
	
	perform<float,region1D,layout1D>();
	perform<double,region1D,layout1D>();
	
	perform<float,region2D,layout2D>();
	perform<double,region2D,layout2D>();
	
	perform<float,region3D,layout3D>();
	perform<double,region3D,layout3D>();
	
}
YOCTO_UNIT_TEST_DONE()
