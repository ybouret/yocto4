#include "yocto/utest/run.hpp"
#include "yocto/cliff/contour2d.hpp"
#include "yocto/cliff/fill.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/cliff/level-set.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	static inline double f2( double x, double y )
	{
		const double dx = (x-1);
		return sin( sqrt( x*x + 1.5 * y*y ) ) + 1/(dx*dx+1);
		
	
	}
	
	
	
	class isoline
	{
	public:
		typedef geom::v2d<double> vtx_t;
		
		explicit isoline() {}
		virtual ~isoline() throw()
		{
		}
		
		void process( const vtx_t &a, const vtx_t &b, const level_set<double>::level &l )
		{
			const int  key    = l.key;
			const bool append = (l.flags != 0);
			l.flags = 1;
			ios::ocstream fp( vformat("iso%d.dat",key), append );
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
	wksp_type w(coord2D(1,1), coord2D(200,200),
				coord2D(0,0), coord2D(0,0),
				vertex_t(-3.5,-3.5),
				vertex_t(3.5,3.5),
				1,1,NULL);
	
	array2D<double>      &d = w[1];
	fill_type::function2  f( cfunctor2(f2) );
	fill_type::with( f, d, d, w.X, w.Y );
	double vmin=0,vmax=0;
	d.get_min_max(vmin,NULL,vmax,NULL);
	std::cerr << "vmin= " << vmin << ", vmax=" << vmax << std::endl;
	d.ppm("iso.ppm", "iso", d, vproc, NULL, vmin, vmax);
	
	level_set<double> levels;
	levels.add(1, -0.5 );
	levels.add(2,  0.5 );
	levels.add(3,  1.0 );
	levels.add(4,  1.5 );
	
	for( size_t i=1; i <= levels.size(); ++i )
	{
		assert( 0 == levels[i].flags );
		std::cerr << "level #" << i << " key=" << levels[i].key << ", value=" << levels[i].value << std::endl;
	}
		
	
	isoline lines;
	contour2D<double>::callback countour_process( &lines, & isoline::process );
	contour2D<double>::compute( d, w.X, w.Y, d, levels, countour_process );
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
