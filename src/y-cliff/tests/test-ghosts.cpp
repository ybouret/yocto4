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
		for( size_t i=1; i <= C.number; ++i )
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
		std::cerr << "min       = " << wksp.region.vmin << std::endl;
		std::cerr << "max       = " << wksp.region.vmax << std::endl;
		std::cerr << "delta     = " << wksp.delta      << std::endl;
		std::cerr << "inv_d     = " << wksp.inv_d      << std::endl;
		std::cerr << "inv_dsq   = " << wksp.inv_dsq    << std::endl;
		
		std::cerr << "#asyncGhosts= " << wksp.async_ghosts << std::endl;
		std::cerr << "#plainGhosts= " << wksp.plain_ghosts << std::endl;
		
		std::cerr << "-------- Plain Ghosts --------" << std::endl;
		for( size_t g = 1; g <= wksp.plain_ghosts; ++g )
		{
			const Ghost &outer = wksp.plain_outer_ghost(g);
			const Ghost &inner = wksp.plain_inner_ghost(g);
			
			std::cerr << "< outer ghost #" << g << " @" << outer.label() << " >" << std::endl;
			display_l(outer);
			std::cerr << "< inner ghost #" << g << " @" << inner.label() << " >" << std::endl;
			display_l(inner);
			
			for( size_t j=wksp.number;j>0;--j)
			{
				Ghost::direct_copy( outer, inner, wksp[j] );
			}
			
		}
		
		wksp.acquire_ghosts_data(wksp.number);
		std::cerr << "-------- ASync Ghosts --------" << std::endl;
		for( size_t g = 1; g <= wksp.async_ghosts; ++g )
		{
			const Ghost &outer = wksp.async_outer_ghost(g);
			const Ghost &inner = wksp.async_inner_ghost(g);
			
			std::cerr << "< outer ghost #" << g << " @" << outer.label() << " >" << std::endl;
			display_l(outer);
			std::cerr << "< inner ghost #" << g << " @" << inner.label() << " >" << std::endl;
			display_l(inner);
			
			for( size_t j=wksp.number;j>0;--j)
			{
				inner.pull( wksp[j], j );
			}
			
			for( size_t j=wksp.number;j>0;--j)
			{
				outer.push( wksp[j], j );
			}
			
		}
		
		
		std::cerr << "-------- Nucleus --------" << std::endl;
		display_l(wksp.nucleus);
		
		
		
		
		
		
		std::cerr << std::endl;
	}
	
	static inline double f1(double)        { return alea<double>(); }
	static inline double f2(double,double) { return alea<double>(); }
	static inline double f3(double,double,double) { return alea<double>(); }
	
}

YOCTO_UNIT_TEST_IMPL(ghosts)
{
	const char  *names[] = { "u", "v" };
	const size_t cnum    = sizeof(names)/sizeof(names[0]);
	
	{
		typedef wksp1D<double,double> w1D_t;
		const w1D_t:: layout_type    L( -10, 10 );
		const w1D_t:: region_type    R( -1, 1 );
		
		ghosts_setup<coord1D> G;
		G.outer.lower.count = 2;
		
		G.outer.upper.count = 1;
		
		G.inner.lower.count = 1;
		
		G.inner.upper.count = 2;
		
		{
			w1D_t W(L,
					G,
					R,
					cnum, names );
			fill<double,double>::function1 F1( cfunctor(f1) );
			for( size_t i=1; i <= W.number; ++i )
				fill<double,double>::with( F1, W[i], W.outline, W.X );
			display_info(W);	
		}
		
		G.outer.lower.async = 1;
		G.inner.upper.async = 1;
		
		{
			w1D_t W(L,
					G,
					R,
					cnum, names );
			display_info(W);
		}
	}
	
	{
		typedef wksp2D<double,double>  w2D_t;
		typedef vertex2D<double>::type v2D_t;
		const w2D_t::layout_type    L( coord2D(-20,-10), coord2D(10,20) );
		const w2D_t::region_type    R( v2D_t(-1,-1), v2D_t(1,1) );
		//const ghosts_infos<coord2D> g_lo( coord2D(1,2), coord2D(0,0) ); //! 1 lower_x, 2 lower_y, not deferred
		//const ghosts_infos<coord2D> g_up( coord2D(3,4), coord2D(1,0) ); //! 3 upper_x, 4 upper_y, upper_x is deffered
		//const ghosts_setup<coord2D> G( g_lo, g_up );
		ghosts_setup<coord2D> G;
		
		G.outer.upper.count = coord2D(2,1); //! 2 at upper_x, 1 at upper_y
		G.outer.upper.async = coord2D(0,1); //! plain on x, async on y
		
		G.inner.lower.count = coord2D(2,1); //! 2 at lower_x, 1 at lower_y
		G.inner.lower.async = coord2D(0,1); //! plain on x, async on y
		
				
		G.outer.lower.count = coord2D(3,1); //!< 3 at lower_x, 1 a lower_y
		G.outer.lower.async = coord2D(0,1); //!< plain on x, async on y
	
		G.inner.upper.count = coord2D(3,1); 
		G.inner.upper.async = coord2D(0,1);
		
		w2D_t W(L,
				G,
				R,
				cnum, names );
		fill<double,double>::function2 F2( cfunctor2(f2) );
		for( size_t i=1; i <= W.number; ++i )
			fill<double,double>::with( F2, W[i], W.outline, W.X, W.Y );
		display_info(W);		
	}
	
	
	{
		typedef wksp3D<double,double>  w3D_t;
		typedef vertex3D<double>::type v3D_t;
		const w3D_t::layout_type    L( coord3D(0,0,0), coord3D(10,15,20) );
		const w3D_t::region_type    R( v3D_t(-1,-1,-1), v3D_t(1,1,1) );
		//const ghosts_infos<coord3D> g_lo( coord3D(1,2,3), coord3D(0,0,1) );
		//const ghosts_infos<coord3D> g_up( coord3D(4,5,6), coord3D(0,0,1) );
		//const ghosts_setup<coord3D> G( g_lo, g_up );
		ghosts_setup<coord3D> G;
		
		
		G.outer.upper.count = coord3D(2,1,1);
		G.outer.upper.async = coord3D(0,0,1);
		G.inner.lower.count = coord3D(2,1,1);
		G.inner.lower.async = coord3D(0,0,1);
		
		G.outer.lower.count = coord3D(3,1,1);
		G.outer.lower.async = coord3D(0,0,1);
		G.inner.upper.count = coord3D(3,1,1);
		G.inner.upper.async = coord3D(0,0,1);
		
		w3D_t W(L,
				G,
				R,
				cnum, names );
		fill<double,double>::function3 F3( cfunctor3(f3) );
		for( size_t i=1; i <= W.number; ++i )
			fill<double,double>::with( F3, W[i], W.outline, W.X, W.Y, W.Z);
		display_info(W);		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
