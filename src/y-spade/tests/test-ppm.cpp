#include "yocto/utest/run.hpp"

#include "yocto/spade/workspace2d.hpp"
#include "yocto/code/utils.hpp"
#include <cmath>

using namespace yocto;
using namespace spade;



template <typename LAYOUT>
static inline void displayLayout( const LAYOUT &L )
{
    std::cerr << "Layout" << L.dims << "D: ";
    std::cerr << L.lower << " -> " << L.upper << " : " << L.width << std::endl;
    std::cerr << "\titems=" << L.items << std::endl;
}

static inline
double vproc( const double &v )
{
	return v;
}

YOCTO_UNIT_TEST_IMPL(ppm)
{
	
	unit_t Nx = 100;
	unit_t Ny = 100;
	const coord2D lo(-Nx,-Ny);
	const coord2D hi(Nx,Ny);
	const composition C(1,1);
	const layout2D    L( lo, hi );
	
	workspace2D<double>       W(C,L);
	const double xfac = 1.0 / Nx;
	const double yfac = 1.0 / Ny;
	
	for( unit_t y=W.lower.y; y <= W.upper.y; ++y )
	{
		const double Y = y * yfac;
		for( unit_t x=W.lower.x; x <= W.upper.x; ++x)
		{
			const double X = x * xfac;
			const double r2 =Y*Y + X*X;
			W[1][y][x] = cos( r2 * 6.0 );
		}
	}
	
	W[1].ppm( "full.ppm", "full", vproc, NULL, -1, 1 );
	
	
	
}
YOCTO_UNIT_TEST_DONE()
