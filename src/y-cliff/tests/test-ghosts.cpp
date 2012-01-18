#include "yocto/utest/run.hpp"


#include "yocto/cliff/wksp1d.hpp"
#include "yocto/cliff/wksp2d.hpp"
#include "yocto/cliff/wksp3d.hpp"
#include "yocto/cliff/fill.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace cliff;

namespace 
{
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
		std::cerr << "min       = " << wksp.region.min << std::endl;
		std::cerr << "max       = " << wksp.region.max << std::endl;
		std::cerr << "delta     = " << wksp.delta      << std::endl;
		std::cerr << "inv_d     = " << wksp.inv_d      << std::endl;
		std::cerr << "inv_dsq   = " << wksp.inv_dsq    << std::endl;
		std::cerr << "ghosts    = " << wksp.ghosts     << std::endl;
		for( size_t i=1; i <= wksp.ghosts; ++i )
		{
			std::cerr << "**** Ghosts #" << i << std::endl;
			std::cerr << "   |_position    : " << wksp.outer_ghost(i).label() << std::endl;
			std::cerr << "   |_outer layout:" << std::endl;
			display_l( wksp.outer_ghost(i) );
			if( WKSP::DIMENSIONS <= 2 )
			{
				const offsets_list &offsets = wksp.outer_ghost(i).offsets;
				std::cerr << "offsets:";
				for( size_t j=1; j <= offsets.size(); ++j )
				{
					std::cerr << ' ' << offsets[j];
				}
				std::cerr << std::endl;
			}
			
			for( size_t j= wksp.cmin; j <= wksp.cmax; ++j )
			{
				wksp.outer_ghost(i).fetch( wksp[j] );
			}
			
		}
		
		std::cerr << std::endl;
	}
	
	static inline double f1(double)        { return alea<double>(); }
	static inline double f2(double,double) { return alea<double>(); }
	static inline double f3(double,double,double) { return alea<double>(); }

}

YOCTO_UNIT_TEST_IMPL(ghosts)
{
	const size_t cmin    = 1;
	const char  *names[] = { "u", "v" };
	const size_t cnum    = sizeof(names)/sizeof(names[0]);
	
	{
		typedef wksp1D<double,double> w1D_t;
		w1D_t W(-10,10,
				1, 2,
				-1, 1,
				cmin, cnum, names );
		fill<double,double>::function1 F1( cfunctor(f1) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F1, W[i], W.outline, W.X );
		display_info(W);		
	}
	
	{
		typedef wksp2D<double,double>  w2D_t;
		typedef vertex2D<double>::type v2D_t;
		w2D_t W(coord2D(-20,-10), coord2D(10,20),
				coord2D(1,2), coord2D(3,4),
				v2D_t(-1,-1), v2D_t(1,1),
				cmin, cnum, names );
		fill<double,double>::function2 F2( cfunctor2(f2) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F2, W[i], W.outline, W.X, W.Y );
		display_info(W);		
	}
	
	{
		typedef wksp3D<double,double>  w3D_t;
		typedef vertex3D<double>::type v3D_t;
		w3D_t W(coord3D(0,0,0), coord3D(10,15,20),
				coord3D(1,2,3), coord3D(4,5,6),
				v3D_t(-1,-1,-1), v3D_t(1,1,1),
				cmin, cnum, names );
		fill<double,double>::function3 F3( cfunctor3(f3) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F3, W[i], W.outline, W.X, W.Y, W.Z);
		display_info(W);		
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
