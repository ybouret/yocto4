#include "yocto/utest/run.hpp"
#include "yocto/cliff/contour2d.hpp"
#include "yocto/cliff/fill.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/cliff/level-set.hpp"
#include "yocto/cliff/rwops.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
	static inline double f2( double x, double y )
	{
		const double dx = (x-1);
		const double dy = (y-1);
		return sin( sqrt( x*x + 1.5 * y*y ) ) + sin(1/(dx*dx+0.3)+1/(dy*dy+0.3));
		
		
	}
	
	
	
	class isoline
	{
	public:
		typedef geom::v2d<double> vtx_t;
		
		explicit isoline() {}
		virtual ~isoline() throw()
		{
		}
		
		void prolog( const level_set<double> &levels )
		{
			for( size_t i= levels.size(); i>0; --i)
			{
				ios::ocstream fp( vformat("iso%d.dat", levels[i].key), false);
				fp("#value=%g\n", levels[i].value );
			}
		}
		
		void process( const vtx_t &a, const vtx_t &b, const level_set<double>::level &l )
		{
			const int  key    = l.key;
			ios::ocstream fp( vformat("iso%d.dat",key), true );
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


YOCTO_UNIT_TEST_IMPL(contour2)
{
	
	
	typedef wksp2D<double,double> wksp_type;
	typedef fill<double,double>   fill_type;
	typedef wksp_type::vertex_t   vertex_t;
	const layout2D               L( coord2D(1,1), coord2D(200,200) );
	const region2D<double>::type R( vertex_t(-4.0,-4.0),  vertex_t(4.0,4.0) );
	wksp_type w(L,
				coord2D(0,0), coord2D(0,0),
				R,
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
		std::cerr << "level #" << i << " key=" << levels[i].key << ", value=" << levels[i].value << std::endl;
	}
	
	
	isoline lines;
	lines.prolog( levels );
	contour2D<double>::callback countour_process( &lines, & isoline::process );
	contour2D<double>::compute( d, w.X, w.Y, d, levels, countour_process );
	
	rwops<double>::save_vtk( "field2.vtk", "field2", "A", d, d, w.region.min, w.delta );
	
	
}
YOCTO_UNIT_TEST_DONE()
