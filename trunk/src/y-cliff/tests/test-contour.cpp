#include "yocto/utest/run.hpp"
#include "yocto/cliff/contour.hpp"
#include "yocto/cliff/fill.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	static inline double f2( double x, double y )
	{
		return sin( sqrt( x*x + 1.5 * y*y ) );
	}



class isoline
{
public:
	typedef geom::v2d<double> vtx_t;
	
	explicit isoline() {}
	virtual ~isoline() throw()
	{
	}
	
	void process( const vtx_t &a, const vtx_t &b, double z )
	{
		ios::ocstream fp( "iso.dat", true );
		fp("%g %g\n", a.x, a.y);
		fp("%g %g\n", b.x, b.y);
	}
	
	
	
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(isoline);
};

	static inline double vproc( const double &x ) 
	{
		return x;
	}
}


YOCTO_UNIT_TEST_IMPL(contour)
{
	
	
	typedef wksp2D<double,double> wksp_type;
	typedef fill<double,double>   fill_type;
	typedef wksp_type::vertex_t   vertex_t;
	wksp_type w(coord2D(1,1), coord2D(50,100),
				coord2D(0,0), coord2D(0,0),
				vertex_t(-2.5,-2.5),
				vertex_t(2.5,2.5),
				1,1,NULL);
	
	array2D<double>      &d = w[1];
	fill_type::function2  f( cfunctor2(f2) );
	fill_type::with( f, d, d, w.X, w.Y );
	d.ppm("iso.ppm", "iso", d, vproc, NULL);
	vector<double> levels;
	levels.push_back( 0 );
	levels.push_back( 0.5 );
	
	hsort( levels );
	isoline                   lines;
	contour<double>::callback countour_process( &lines, & isoline::process );
	contour<double>::compute( d, w.X, w.Y, d, levels, countour_process );
	
	
	
}
YOCTO_UNIT_TEST_DONE()
