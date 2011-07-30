#include "yocto/utest/run.hpp"
#include "yocto/spade/workspace2d.hpp"
#include "yocto/geom/bresenham.hpp"

using namespace yocto;
using namespace spade;


namespace  {
	
	template <typename T>
	struct set_inside
	{
		array2D<T> *a;
		void on( unit_t x, unit_t y )
		{
			assert( a );
			(*a)[y][x] = 1;
		}
		
	};
	
	template <typename T>
	static inline 
	double vproc( const T &x )
	{
		return double(x);
	}
	
	template <typename T>
	static inline void perform_test(unit_t r)
	{
		
		workspace2D<T> w( composition(1,1), layout2D( coord2D(-(r+1),-(r+1)), coord2D( r+1, r+1 ) ));
		array2D<T> &a = w[1];
		std::cerr << "lower=" << a.lower << ", upper=" << a.upper << std::endl;
		a.ld(0);
		set_inside<T> proc = { &a };
		typename gfx::bresenham<unit_t>::callback_type cb(  &proc, & set_inside<T>::on );
		gfx::bresenham<unit_t>::disk( 0,0, r, cb );
		
		a.ppm("disk.ppm", "disk", vproc<T>, NULL, 0,1);
		
	}
	
}
YOCTO_UNIT_TEST_IMPL(bresenham)
{
	perform_test<float>(10);
}
YOCTO_UNIT_TEST_DONE()
