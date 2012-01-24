#include "yocto/utest/run.hpp"


#include "yocto/cliff/wksp1d.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/cliff/wksp3d.hpp"

#include "yocto/cliff/laplacian.hpp"
#include "yocto/cliff/fill.hpp"

#include "yocto/math/complex.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace cliff;
using namespace math;
using namespace geom;

namespace {
	
	
	template<typename LAYOUT>
	static inline void display_l( const LAYOUT &L )
	{
		std::cerr << "-- " << LAYOUT::DIMENSIONS << "D" << std::endl;
		std::cerr << "-- lower= " << L.lower << std::endl;
		std::cerr << "-- upper= " << L.upper << std::endl;
		std::cerr << "-- width= " << L.width << std::endl;
		std::cerr << "-- items= " << L.items << std::endl;
	}
	
	static inline void display_c( const components &C )
	{
		for( size_t i=1; i <= C.number; ++i )
		{
			std::cerr << "component #" << i << " = " << C.name(i) << std::endl;
		}
	}
	
	template <typename WKSP>
	static inline void display_info( const WKSP &wksp)
	{
		std::cerr << "---- layout:" << std::endl;
		display_l(wksp);
		std::cerr << "---- outline: " << std::endl;
		display_l(wksp.outline);
		std::cerr << "---- components: " << std::endl;
		display_c(wksp);
		std::cerr << "min     = " << wksp.region.min << std::endl;
		std::cerr << "max     = " << wksp.region.max << std::endl;
		std::cerr << "delta   = " << wksp.delta      << std::endl;
		std::cerr << "inv_d   = " << wksp.inv_d      << std::endl;
		std::cerr << "inv_dsq = " << wksp.inv_dsq    << std::endl;
		
		std::cerr << "sizeof(wksp::data_block)=" << sizeof( typename WKSP::data_block) << std::endl;
		std::cerr << "sizeof(wksp::axis_type) =" << sizeof( typename WKSP::axis_type)  << std::endl;
		std::cerr << std::endl;
	}
	
}

static inline double f2( double x, double y )
{
	return sin(x+y);
}

static inline double vproc( const double &x )
{
	return x;
}

static inline complex<float> f1( double x )
{
	return sin(x);
}

static inline complex<double> f3( float x, float y, float z )
{
	return sin(x+y+z);
}


static inline void proc1( array< complex<float> > &var, void * )
{
	
}

YOCTO_UNIT_TEST_IMPL(wksp)
{
	{	
		const layout1D L(-10,20);
		const region1D<double>::type R(-2.0,4.0);
		//const ghosts_infos<coord1D>  g_lo(1,0);
		//const ghosts_infos<coord1D>  g_up(2,0);
		//const ghosts_setup<coord1D>  G( g_lo, g_up );
		
		ghosts_setup<coord1D> G;
		wksp1D< complex<float>, double>  w1(L, 
											G, 
											R,
											2,NULL
											);
		
		display_info( w1 );
		fill< complex<float>, double >::function1 F( cfunctor(f1) );
		fill< complex<float>, double >::with(F,w1[1],w1.outline,w1.X);
		
		
		std::cerr << "X=" << w1.X << std::endl;
		{
			ios::ocstream fp( "w1.dat", false );
			for( unit_t x = w1.lower; x <= w1.upper; ++x )
			{
				fp("%g %g\n", w1.X[x], w1[1][x].re);
			}
		}
		layout1D in1 = w1.outline.inside();
		std::cerr << "---- inside:" << std::endl;
		display_l(in1);
		laplacian<complex<float>, double>::compute( w1[2], 1, w1[1], w1.inv_dsq, in1);
		{
			ios::ocstream fp( "l1.dat", false );
			for( unit_t x = in1.lower; x <= in1.upper; ++x )
			{
				fp("%g %g\n", w1.X[x], w1[2][x].re);
			}
		}
		
		vector<size_t> cid;
		for( size_t i=1; i <= w1.number; ++i ) cid.push_back(i);
		w1.check_indices(cid);
		vector< complex<float> > var( cid.size(), 0 );
		offsets_list off1;
		layout1D::load_offsets( off1, w1.outline, w1.outline );
		for( size_t i=off1.size();i>0;--i)
		{
			const size_t j = off1[i];
			w1.query(var, cid, j);
			for( size_t k=var.size(); k >0; --k )
			{
				var[k] = complex<float>( alea<float>(), alea<float>() );
			}
			w1.store(var,cid,j);
		}
		
	}
	
	{
		const char  *varnames[] = { "u", "v", "w" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		const layout2D              L( coord2D(-100,-100), coord2D(200,200) );
		const region2D<float>::type R( v2d<float>(-2,-2), v2d<float>(4,4) );
		
		//const ghosts_infos<coord2D> g_lo( coord2D(0,1), coord2D(0,0) );
		//const ghosts_infos<coord2D> g_up( coord2D(0,2), coord2D(0,0) );
		//const ghosts_setup<coord2D> G( g_lo, g_up );
		
		ghosts_setup<coord2D> G;
		wksp2D< double, float > w2(L,
								   G,
								   R,
								   varcount, varnames );
		display_info( w2 );
		std::cerr << "X=" << w2.X << std::endl;
		std::cerr << "Y=" << w2.Y << std::endl;
		fill< double, float >::function2 F( cfunctor2(f2) );
		fill<double,float>::with( F, w2["u"], w2.outline, w2.X, w2.Y );
		
		w2["u"].ppm("w2.ppm","u",w2,vproc,NULL,-1,1);
		layout2D in2 = w2.outline.inside();
		std::cerr << "---- inside:" << std::endl;
		display_l(in2);
		laplacian<double,float>::compute( w2["v"], 1, w2["u"], w2.inv_dsq, in2 );
		w2["v"].ppm("l2.ppm", "v",in2,vproc,NULL,-1,1);
		
		w2["w"].set_all( w2.outline, 0 );
		w2["w"].set( w2["u"], in2 );
		w2["w"].add( w2["v"], in2 );
		offsets_list off2;
		vector<size_t> cid;
		cid.push_back( w2("u") );
		cid.push_back( w2("v") );
		vector<double> var( cid.size(), 0 );
		layout2D::load_offsets( off2, w2.outline, w2.outline );
		for( size_t i=off2.size();i>0;--i)
		{
			const size_t j = off2[i];
			w2.query(var, cid, j);
			for( size_t k=var.size(); k >0; --k )
			{
				var[k] = alea<double>();
			}
			w2.store(var,cid,j);
		}
		
	}
	
	{
		const char  *varnames[] = { "A", "B", "C", "D" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		const layout3D L( coord3D(-20,-20,-20), coord3D(40,40,40) );
		const region3D<float>::type R( v3d<float>(-1,-1,-1), v3d<float>(2,2,2) );
		
		//const ghosts_infos<coord3D> g_lo(coord3D(0,0,1), coord3D());
		//const ghosts_infos<coord3D> g_up(coord3D(0,0,2), coord3D());
		//const ghosts_setup<coord3D> G(g_lo,g_up);
		
		ghosts_setup<coord3D> G;
		wksp3D< complex<double>, float > w3(L,
											G,
											R,
											varcount, varnames );
		display_info( w3 );
		std::cerr << "X=" << w3.X << std::endl;
		std::cerr << "Y=" << w3.Y << std::endl;
		std::cerr << "Z=" << w3.Z << std::endl;
		fill< complex<double>, float >::function3 F( cfunctor3(f3) );
		fill< complex<double>, float >::with( F, w3["A"], w3.outline, w3.X, w3.Y, w3.Z );
		
		layout3D in3 = w3.outline.inside();
		
		laplacian< complex<double>, float>::compute( w3["B"], 1, w3["A"],  w3.inv_dsq, in3 );
		w3["A"].muladd( 1, w3["B"], in3 );
		
		offsets_list   off3;
		vector<size_t> cid;
		cid.push_back( w3("A") );
		cid.push_back( w3("C") );
		vector<complex<double> > var( cid.size(), 0 );
		layout3D::load_offsets( off3, w3.outline, w3.outline );
		for( size_t i=off3.size();i>0;--i)
		{
			const size_t j = off3[i];
			w3.query(var, cid, j);
			for( size_t k=var.size(); k >0; --k )
			{
				var[k] = complex<double>(alea<double>(),alea<double>());
			}
			w3.store(var,cid,j);
		}
		
	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
