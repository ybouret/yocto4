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
		for( size_t i=C.cmin; i <= C.cmax; ++i )
		{
			std::cerr << "component #" << i << " = " << C.get_name(i) << std::endl;
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

YOCTO_UNIT_TEST_IMPL(wksp)
{
	{	
		wksp1D< complex<float>, double>  w1( -10, 20, 
											1, 2, 
											-2.0, 4.0,
											1,2,NULL
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
	}
	
	{
		const char  *varnames[] = { "u", "v", "w" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		wksp2D< double, float > w2( coord2D(-100,-100), coord2D(200,200),
								   coord2D(0,1), coord2D(0,2),
								   v2d<float>(-2,-2), v2d<float>(4,4),
								   1,varcount, varnames );
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
		
		w2["w"].linear_type::set( w2.outline, 0 );
		w2["w"].set( w2["u"], in2 );
		w2["w"].add( w2["v"], in2 );
		
	}
	
	{
		const char  *varnames[] = { "A", "B", "C", "D" };
		const size_t varcount   = sizeof(varnames)/sizeof(varnames[0]);
		wksp3D< complex<double>, float > w3(coord3D(-20,-20,-20), coord3D(40,40,40),
											coord3D(0,0,1), coord3D(0,0,2),
											v3d<float>(-1,-1,-1), v3d<float>(2,2,2),
											0,varcount, varnames );
		display_info( w3 );
		std::cerr << "X=" << w3.X << std::endl;
		std::cerr << "Y=" << w3.Y << std::endl;
		std::cerr << "Z=" << w3.Z << std::endl;
		fill< complex<double>, float >::function3 F( cfunctor3(f3) );
		fill< complex<double>, float >::with( F, w3["A"], w3.outline, w3.X, w3.Y, w3.Z );
		
		layout3D in3 = w3.outline.inside();
		
		laplacian< complex<double>, float>::compute( w3["B"], 1, w3["A"],  w3.inv_dsq, in3 );
		w3["A"].muladd( 1, w3["B"], in3 );
	}
	
	
	
	
}
YOCTO_UNIT_TEST_DONE()
