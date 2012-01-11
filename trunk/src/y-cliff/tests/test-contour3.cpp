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
		return (x2+sin(y2)) * exp( -2.0 * sqrt(r2) );
	}
	
	class isosurf
	{
	public:
		typedef level_set<double>::KEY KEY;
		typedef triangle3D<double>     T3D;
		
		explicit isosurf() throw() : tdb()
		{
		}
		
		virtual ~isosurf() throw()
		{
		}
		
		typedef map<KEY,vector<T3D> > tdb_type;
		tdb_type tdb;
		
		void prolog( const level_set<double> &levels )
		{
			for( size_t i=1; i <= levels.size(); ++i )
			{
				const vector<T3D> tmp;
				if( ! tdb.insert( levels[i].key, tmp ) )
					throw exception("unexpected prolog failure");
			}
		}
		
		
		void process( const T3D &t, const level_set<double>::level &l )
		{
			vector<T3D> *pv = tdb.search( l.key );
			if( !pv ) throw exception("invalid level key");
			//std::cout << "recv: " << t.p0 << " " << t.p1 << " " << t.p2 << std::endl;
			pv->push_back( t );
		}
		
		void epilog(const level_set<double> &levels )
		{
			for( size_t i=1; i <= levels.size(); ++i )
			{
				const level_set<double>::level &l = levels[i];
				vector<T3D> *pv = tdb.search( l.key );
				if( !pv ) throw exception("invalid level key");
				

				const string &fn = vformat( "iso%d.vtk", l.key );
				rwops<double>::save_vtk( fn, "iso surface", *pv );
				
			}
		}
		
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(isosurf);
	};
	
	
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
	
	rwops<double>::save_vtk("field3.vtk", "example field", "A", d, w, w.region.min, w.delta );
	
	double vmin=0,vmax=0;
	d.get_min_max(vmin,NULL,vmax,NULL);
	
	level_set<double> levels;
	levels.add(0, 0.01 );
	
	isosurf                     surfaces;
	surfaces.prolog( levels );
	
	contour3D<double>::callback proc( &surfaces, & isosurf::process);
	
	{
		triangle3D<double> tmp;
		tmp.p0 = vertex_t(1,2,3);
		tmp.p1 = vertex_t(3,4,5);
		tmp.p2 = vertex_t(6,7,8);
		proc( tmp, levels[1] );
	}
		
	contour3D<double>::compute( d, w.X, w.Y, w.Z, d, levels, proc );
	
	surfaces.epilog( levels );
	
}
YOCTO_UNIT_TEST_DONE()
