#ifndef YOCTO_CLIFF_WORKSPACE_INCLUDED
#define YOCTO_CLIFF_WORKSPACE_INCLUDED 1

#include "yocto/cliff/types.hpp"
#include "yocto/cliff/components.hpp"

#include "yocto/type-traits.hpp"
#include "yocto/code/static-check.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/memory/global.hpp"

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
		template <typename> class ARRAY,   // layout[1|2|3]D
		typename                  U,        // float/double for real coordinates
		template <typename> class REGION    // region with associated vertex
		>
		class workspace : public  ARRAY<T>::layout_type, public components
		{
		public:
			//==================================================================
			// compute types
			//==================================================================
			YOCTO_ARGUMENTS_DECL_T; //!< for data handling
			typedef ARRAY<T>                           array_type;
			typedef typename array_type::layout_type   layout_type;
			typedef typename layout_type::coord_t      coord_t;
			typedef typename layout_type::const_coord  const_coord;
			typedef typename layout_type::param_coord  param_coord;
			typedef U                                  real_t;
			typedef typename REGION<U>::type           region_type;
			typedef typename region_type::vertex_t     vertex_t;
			typedef typename region_type::param_vertex param_vertex;
			typedef memory::global                     memory_kind;
			
			static  const    size_t                    DIMENSIONS = layout_type::DIMENSIONS;
			
			//==================================================================
			// blocks for memory handling per species
			//==================================================================
			class block : public object, public array_type
			{
			public:
				explicit block( const layout_type &L ) :
				array_type(L),
				n( this->items ),
				p( memory::kind<memory_kind>::acquire_as<type>(n) )
				{
					this->link(p);
				}
				
				virtual ~block() throw()
				{
					memory::kind<memory_kind>::release_as<type>(p,n);
				}
				
				
			private:
				size_t     n;      //!< this->items
				type      *p;      //!< for linking
				YOCTO_DISABLE_COPY_AND_ASSIGN(block);
			};
			typedef shared_ptr<block> block_ptr;
			
			
			const layout_type outline; //!< original layout+ghosts
			const region_type region;  //!< real space associated to layout (NOT outline)
			
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
			layout_type( lo, hi ),
			components(a,b,names_list),
			outline( compute_outline( *this, ghosts_lo, ghosts_up) ),
			region(vmin,vmax),
			blocks( this->size, as_capacity )
			{
				YOCTO_STATIC_CHECK(DIMENSIONS==region_type::DIMENSIONS,cliff_workspace);
				//--------------------------------------------------------------
				// allocate memory for each component
				//--------------------------------------------------------------
				for( size_t i=1; i <= size; ++i )
				{
					const block_ptr blk( new block( outline ) );
					blocks.push_back( blk );
				}
			}
			
			
			virtual ~workspace() throw()
			{
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
			vector<block_ptr> blocks;
			
			static inline layout_type compute_outline( const layout_type &L, param_coord ghosts_lo, param_coord ghosts_up )
			{
				workspace_base::check( &ghosts_lo, DIMENSIONS, "lower ghosts" );
				workspace_base::check( &ghosts_up, DIMENSIONS, "upper ghosts" );
				const_coord out_lo = L.lower - ghosts_lo;
				const_coord out_up = L.upper + ghosts_up;
				return layout_type(out_lo,out_up);
			}
		};
	}
	
}

#endif
