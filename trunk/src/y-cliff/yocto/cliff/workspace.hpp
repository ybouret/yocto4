#ifndef YOCTO_CLIFF_WORKSPACE_INCLUDED
#define YOCTO_CLIFF_WORKSPACE_INCLUDED 1

#include "yocto/cliff/types.hpp"
#include "yocto/cliff/components.hpp"
#include "yocto/type-traits.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		struct workspace_base
		{
			static void check( const void *ghosts_value, size_t n, const char *ctx );
			
		};
		
		template <
		typename                  T,        // real/complex...
		typename                  LAYOUT,   // layout[1|2|3]D
		typename                  U,        // float/double for real coordinates
		template <typename> class REGION    // region with associated vertex
		>
		class workspace : public LAYOUT, public components
		{
		public:
			//==================================================================
			// compute types
			//==================================================================
			YOCTO_ARGUMENTS_DECL_T; //!< for data handling
			typedef LAYOUT                             layout_type;
			typedef typename LAYOUT::coord_t           coord_t;
			typedef typename LAYOUT::const_coord       const_coord;
			typedef typename LAYOUT::param_coord       param_coord;
			typedef U                                  real_t;
			typedef typename REGION<U>::type           region_type;
			typedef typename region_type::vertex_t     vertex_t;
			typedef typename region_type::param_vertex param_vertex;
			static  const    size_t                    DIMENSIONS = LAYOUT::DIMENSIONS;
			
			
			const layout_type outline; //!< LAYOUT+ghosts
			const region_type region;
			
			//! construct a workspace
			/**
				
			 */
			explicit workspace(param_coord  lo, 
							   param_coord  hi, 
							   param_coord  ghosts_lo, 
							   param_coord  ghosts_up,
							   param_vertex vmin,
							   param_vertex vmax,
							   size_t       a,
							   size_t       b,
							   const char  *names_list[]
							   ) :
			LAYOUT( lo, hi ),
			components(a,b,names_list),
			outline( compute_outline( *this, ghosts_lo, ghosts_up) ),
			region(vmin,vmax)
			{
				YOCTO_STATIC_CHECK(DIMENSIONS==region_type::DIMENSIONS,cliff_workspace);
			}
			
			
			virtual ~workspace() throw()
			{
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
			static inline LAYOUT compute_outline( const LAYOUT &L, param_coord ghosts_lo, param_coord ghosts_up )
			{
				workspace_base::check( &ghosts_lo, DIMENSIONS, "lower ghosts" );
				workspace_base::check( &ghosts_up, DIMENSIONS, "upper ghosts" );
				const_coord out_lo = L.lower - ghosts_lo;
				const_coord out_up = L.upper + ghosts_up;
				return LAYOUT(out_lo,out_up);
			}
		};
	}
	
}

#endif
