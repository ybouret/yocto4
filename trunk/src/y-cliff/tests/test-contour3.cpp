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
	
	static inline float f3v( double x, double y, double z )
	{
		const double x2 = x*x;
		const double y2 = y*y;
		const double z2 = z*z;
		const double r2 = x2 + y2 + z2;
		return (x2+cos(y2)) * exp( -2.0 * sqrt(r2) );
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
	const   wksp_type::layout_type L( coord3D(1,1,1), coord3D(50,50,50) );
	const   wksp_type::region_type R( vertex_t(-4.0,-4.0,-4.0),vertex_t(4.0,4.0,4.0));
	
	//const ghosts_infos<coord3D>    g( coord3D(0,0,0), coord3D(0,0,0) );
	//const ghosts_setup<coord3D>    G(g,g);
	const ghosts_setup<coord3D> G;
	const char                 *vars[] = { "u", "v" };
	wksp_type w(L,
				G,
				R,
				2,vars);
	
	array3D<double> &u = w["u"];
	array3D<double> &v = w["v"];
	fill_type::function3 fu( cfunctor3(f3) );
	fill_type::function3 fv( cfunctor3(f3v) );

	fill_type::with( fu, u, w, w.X, w.Y, w.Z);
	fill_type::with( fv, v, w, w.X, w.Y, w.Z);
	vector<size_t> cid;
	cid.push_back( 1 );
	cid.push_back( 2 );
	
	rwops<double>::save_vtk("field3.vtk", "example field", w, cid, w);
	
	double vmin=0,vmax=0;
	u.get_min_max(vmin,NULL,vmax,NULL);
	
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
		
	contour3D<double>::compute( u, w.X, w.Y, w.Z, u, levels, proc );
	
	surfaces.epilog( levels );
	
}
YOCTO_UNIT_TEST_DONE()
