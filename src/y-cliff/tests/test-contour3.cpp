#include "yocto/utest/run.hpp"
#include "yocto/cliff/contour3d.hpp"
#include "yocto/cliff/fill.hpp"
#include "yocto/cliff/wksp3d.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/cliff/level-set.hpp"
#include "yocto/cliff/rwops.hpp"

using namespace yocto;
using namespace cliff;

namespace
{
	static inline float f3( double x, double y, double z )
	{
		const double x2 = x*x;
		const double y2 = y*y;
		const double z2 = z*z;
		const double r2 = x2 + y2 + z2;
		return z2 * exp( -sqrt(r2) );
	}
	
}

YOCTO_UNIT_TEST_IMPL(contour3)
{
	typedef wksp3D<double,double> wksp_type;
	typedef fill<double,double>   fill_type;
	typedef wksp_type::vertex_t   vertex_t;
	wksp_type w(coord3D(1,1,1), coord3D(50,50,50),
				coord3D(0,0,0), coord3D(0,0,0),
				vertex_t(-4.0,-4.0,-4.0),
				vertex_t(4.0,4.0,4.0),
				1,1,NULL);
	
	array3D<double> &d = w[1];
	fill_type::function3 f( cfunctor3(f3) );
	fill_type::with( f, d, w, w.X, w.Y, w.Z);
	
	rwops<double>::save_vtk("field3.vtk", w.get_name(1), d, w, w.region.min, w.delta );
	
	
	
}
YOCTO_UNIT_TEST_DONE()
