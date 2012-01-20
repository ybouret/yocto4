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
		//std::cerr << "-- " << LAYOUT::DIMENSIONS << "D" << std::endl;
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
	static inline void display_info( WKSP &wksp)
	{
		typedef typename WKSP::ghost_type Ghost;
		
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
			const Ghost &G = wksp.outer_ghost(i);
			const Ghost &H = wksp.inner_ghost(i);
			std::cerr << "**** Ghosts #" << i << std::endl;
			std::cerr << "   |_outer position: " << G.label() << std::endl;
			std::cerr << "   |_outer deferred: " << G.deferred << std::endl;
			std::cerr << "   |_outer layout:" << std::endl;
			display_l( G );
			if( WKSP::DIMENSIONS <= 2 )
			{
				const offsets_list &offsets = G.offsets;
				std::cerr << "offsets:";
				for( size_t j=1; j <= offsets.size(); ++j )
				{
					std::cerr << ' ' << offsets[j];
				}
				std::cerr << std::endl;
			}
			
			std::cerr << "   |_inner position: " << H.label() << std::endl;
			std::cerr << "   |_outer deferred: " << H.deferred << std::endl;
			std::cerr << "   |_inner layout:" << std::endl;
			display_l(H);
			if( WKSP::DIMENSIONS <= 2 )
			{
				const offsets_list &offsets = H.offsets;
				std::cerr << "offsets:";
				for( size_t j=1; j <= offsets.size(); ++j )
				{
					std::cerr << ' ' << offsets[j];
				}
				std::cerr << std::endl;
			}
			
			
			std::cerr << "   |_pull/push..." << std::endl;
			H.acquire_data();
			G.acquire_data();
			for( size_t j= wksp.cmin; j <= wksp.cmax; ++j )
			{
				memset( G.data, 0, G.bytes );
				G.pull( wksp[j] );
				G.push( wksp[j] );
				
				memset( H.data, 0, H.bytes );
				H.pull( wksp[j] );
				H.push( wksp[j] );
			}
			
		}
		std::cerr << "---- nucleus:" << std::endl;
		display_l(wksp.nucleus);
		
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
		const w1D_t:: layout_type L( -10, 10 );
		const w1D_t:: region_type R( -1, 1 );
		const ghosts_info<coord1D>  g_lo( 1, 0 ); //! one, not deferred
		const ghosts_info<coord1D>  g_up( 2, 1 ); //! two, deferred
		const ghosts_setup<coord1D> G( g_lo, g_up );
		w1D_t W(L,
				G,
				R,
				cmin, cnum, names );
		fill<double,double>::function1 F1( cfunctor(f1) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F1, W[i], W.outline, W.X );
		display_info(W);		
	}
	
	{
		typedef wksp2D<double,double>  w2D_t;
		typedef vertex2D<double>::type v2D_t;
		const w2D_t::layout_type L( coord2D(-20,-10), coord2D(10,20) );
		const w2D_t::region_type R( v2D_t(-1,-1), v2D_t(1,1) );
		const ghosts_info<coord2D>  g_lo( coord2D(1,2), coord2D(0,0) ); //! 1 lower_x, 2 lower_y, not deferred
		const ghosts_info<coord2D>  g_up( coord2D(3,4), coord2D(1,0) ); //! 3 upper_x, 4 upper_y, upper_x is deffered
		const ghosts_setup<coord2D> G( g_lo, g_up );
		w2D_t W(L,
				G,
				R,
				cmin, cnum, names );
		fill<double,double>::function2 F2( cfunctor2(f2) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F2, W[i], W.outline, W.X, W.Y );
		display_info(W);		
	}
	
	{
		typedef wksp3D<double,double>  w3D_t;
		typedef vertex3D<double>::type v3D_t;
		const w3D_t::layout_type    L( coord3D(0,0,0), coord3D(10,15,20) );
		const w3D_t::region_type    R( v3D_t(-1,-1,-1), v3D_t(1,1,1) );
		const ghosts_info<coord3D>  g_lo( coord3D(1,2,3), coord3D(0,0,1) );
		const ghosts_info<coord3D>  g_up( coord3D(4,5,6), coord3D(0,0,1) );
		const ghosts_setup<coord3D> G( g_lo, g_up );
		w3D_t W(L,
				G,
				R,
				cmin, cnum, names );
		fill<double,double>::function3 F3( cfunctor3(f3) );
		for( size_t i=W.cmin; i <= W.cmax; ++i )
			fill<double,double>::with( F3, W[i], W.outline, W.X, W.Y, W.Z);
		display_info(W);		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
