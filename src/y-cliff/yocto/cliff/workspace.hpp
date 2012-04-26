#ifndef YOCTO_CLIFF_WORKSPACE_INCLUDED
#define YOCTO_CLIFF_WORKSPACE_INCLUDED 1

#include "yocto/cliff/array1d.hpp"
#include "yocto/cliff/components.hpp"
#include "yocto/cliff/ghosts.hpp"

#include "yocto/type-traits.hpp"
#include "yocto/code/static-check.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/memory/global.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		//! utilities for workspace setup
		struct workspace_base
		{
			static void check_ghosts( const void *outer_lo, const void *outer_up, const void *inner_lo, const void *inner_up,  const void *w, size_t n );
			static void check_widths( const unit_t *w, size_t n );
			static void check_indices( const array<size_t> &cid, const components &cdb );
		};
		
		
		//! template to provide memory to arrays
		template <typename T,template <class> class ARRAY>
		class block : public object, public ARRAY<T>
		{
		public:
			explicit block( const typename ARRAY<T>::layout_type &L ) :
			ARRAY<T>( L ),
			n( this->items ),
			p( memory::kind<memory::global>::acquire_as<T>(n) )
			{
				this->link(p);
			}
			
			virtual ~block() throw()
			{
				memory::kind<memory::global>::release_as<T>(p,n);
			}
			
		private:
			size_t n;
			T     *p;
			YOCTO_DISABLE_COPY_AND_ASSIGN(block);
		};
		
		
		//! heavily templated workspace
		/**
		 Manage arrays, region and ghosts.
		 */
		template <
		typename                  T,       // real/complex...
		template <typename> class ARRAY,   // layout[1|2|3]D
		typename                  U,       // float/double for real coordinates
		template <typename> class REGION   // region with associated vertex
		>
		class workspace : public  ARRAY<T>::layout_type, public components
		{
		public:
			//==================================================================
			// compute types
			//==================================================================
			YOCTO_ARGUMENTS_DECL_T; 
			typedef workspace<T,ARRAY,U,REGION>        wksp_type;
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
			typedef block<U,array1D>                   axis_type;
			typedef ghosts_setup<coord_t>              ghosts_type;
			static  const    size_t                    DIMENSIONS = layout_type::DIMENSIONS;
			
			//==================================================================
			// blocks for memory handling per component
			//==================================================================
			typedef block<T,ARRAY>         data_block;		
			typedef shared_ptr<data_block> data_block_ptr;
			
			//==================================================================
			// ghosts
			//==================================================================
			typedef ghost<T,coord_t>       ghost_type;
			typedef shared_ptr<ghost_type> ghost_ptr;
			
			const layout_type outline;      //!< original layout+ghosts
			const region_type region;       //!< real space associated to layout (NOT outline)
			const vertex_t    delta;        //!< step for each dimension
			const vertex_t    inv_d;        //!< 1/delta
			const vertex_t    inv_dsq;      //!< 1/delta^2
			const layout_type nucleus;      //!< original layout - asynchronous inner ghosts
			const size_t      plain_ghosts; //!< number of plain ghosts (outer,inner)
			const size_t      async_ghosts; //!< number of async hosts (outer,inner)
			
			//! construct a workspace
			/**
			 \param L working layout
			 \param G ghosts information
			 \param R working region
			 */
			explicit workspace(const layout_type &L,
							   const ghosts_type &G,
							   const region_type &R,
							   size_t       num,
							   const char  *names_list[]
							   ) :
			layout_type( L ),
			components(num,names_list),
			outline( compute_outline( *this, G) ),
			region(R),
			delta(),
			inv_d(),
			inv_dsq(),
			nucleus( *this ),
			plain_ghosts(0),
			async_ghosts(0),
			blocks( this->number, as_capacity ),
			vaxis( DIMENSIONS, as_capacity ),
			axis_(NULL),
			plain_outer_ghosts(),
			plain_inner_ghosts(),
			async_outer_ghosts(),
			async_inner_ghosts()
			{
				YOCTO_STATIC_CHECK(DIMENSIONS==region_type::DIMENSIONS,cliff_workspace);
				//--------------------------------------------------------------
				// post-compute delta
				//--------------------------------------------------------------
				{
					U             *d   = (U *           ) &delta;
					U             *id  = (U *           ) &inv_d;
					U             *id2 = (U *           ) &inv_dsq;
					const  unit_t *w   = (const unit_t *) &(this->width);
					const U       *l   = (const U *     ) &(region.length);
					workspace_base::check_widths(w,DIMENSIONS);
					for( size_t i=0; i < DIMENSIONS; ++i )
					{
						const U del = (d[i]  = l[i] / ( w[i]-1));
						id[i]  = U(1)/del;
						id2[i] = U(1)/(del*del); 
					}
				}
				
				
				//--------------------------------------------------------------
				// allocate memory for each component
				//--------------------------------------------------------------
				for( size_t i=1; i <= this->number; ++i )
				{
					const data_block_ptr blk( new data_block( outline ) );
					blocks.push_back( blk );
				}
				
				//--------------------------------------------------------------
				// allocate memory for each axis, and compute values
				//--------------------------------------------------------------
				{
					const unit_t *lo  = (const unit_t *) & (outline.lower);
					const unit_t *hi  = (const unit_t *) & (outline.upper);
					const U      *d   = (const U *) &delta;
					const U      *o   = (const U *) &region.vmin;
					const unit_t *k   = (const unit_t *) & (this->lower);
					for( size_t i=0; i < DIMENSIONS; ++i )
					{
						//-- allocate the axis
						const unit_t   jlo = lo[i];
						const unit_t   jhi = hi[i];
						const layout1D al( jlo, jhi );
						axis_ptr       ax( new axis_type( al ) );
						vaxis.push_back(ax);
						axis_type &a   = *ax;
						const U      o_i = o[i];
						const U      d_i = d[i];
						const unit_t k_i = k[i];
						for( unit_t j=jlo; j <= jhi; ++j )
						{
							a[j] = o_i + d_i * (j-k_i);
						}
					}
					axis_ = & vaxis[1];
				}
				
				//--------------------------------------------------------------
				// create ghosts and compute nucleus from the
				// inner deferred ghosts
				//--------------------------------------------------------------
				create_ghosts(G);
			}
			
			
			virtual ~workspace() throw()
			{
			}
			
			//! return an axis
			/**
			 \param idim 0..DIMENSIONS-1
			 */
			inline const axis_type & axis(size_t idim) const throw()
			{
				assert(idim<DIMENSIONS);
				return *axis_[idim];
			}
			
			//! returns an array of valid component
			/**
			 \param c in [1..number] of components
			 */
			inline array_type & operator[]( size_t c ) throw()
			{
				assert(c>=1);
				assert(c<=this->number);
				return * blocks[c];
			}
			
			//! returns a const array of valid component
			/**
			 \param c in [1..number] of components
			 */
			inline const array_type & operator[]( const size_t c ) const throw()
			{
				assert(c>=1);
				assert(c<=this->number);
				return * blocks[c];
			}
			
			
			//! returns an array of valid component
			/**
			 \param id a valid component id
			 */
			inline array_type & operator[]( const string &id )
			{
				const components &comp = *this;
				return * blocks[ comp(id) ];
			}
			
			//! returns an array of valid component
			/**
			 \param id a valid component id
			 */
			inline const array_type & operator[]( const string &id ) const throw()
			{
				const components &comp = *this;
				return * blocks[ comp(id) ];
			}
			
			//! returns an array of valid component
			/**
			 \param id a valid component id
			 */
			inline array_type & operator[]( const char *id )
			{
				const components &comp = *this;
				return * blocks[ comp(id) ];
			}
			
			//! returns an array of valid component
			/**
			 \param id a valid component id
			 */
			inline const array_type & operator[]( const char *id ) const throw()
			{
				const components &comp = *this;
				return * blocks[ comp(id) ];
			}
			
			//! check that the indices are valid
			inline void check_indices( const array<size_t> &cid )	
			{
				workspace_base::check_indices( cid, *this );
			}
			
			//! query and array of variables
			/**
			 \param var vector or float/double/complex values
			 \param cid vector of components indices
			 \param offset offset to query in this->outline.
			 */
			inline void query( array<T> &var, const array<size_t> &cid, size_t offset ) const throw()
			{
				assert( offset < this->outline.items );
				assert( var.size() >= cid.size() );
				const wksp_type &field = *this;
				for( size_t j = cid.size(); j > 0 ; --j )
				{
					var[j] = field[ cid[j] ].entry[offset];
				}
			}
			
			//! store an array of variables
			/**
			 \param var vector or float/double/complex values
			 \param cid vector of components indices
			 \param offset offset to store in this->outline.
			 */
			inline void store( const array<T> &var, const array<size_t> &cid, size_t offset ) throw()
			{
				assert( offset < this->outline.items );
				assert( var.size() >= cid.size() );
				wksp_type &field = *this;
				for( size_t j = cid.size(); j > 0 ; --j )
				{
					field[ cid[j] ].entry[offset] = var[j];
				}
			}
			
			//!
			inline const ghost_type &plain_outer_ghost( size_t ghost_index ) const throw() 
			{
				assert( ghost_index > 0 ); assert( ghost_index <= plain_ghosts );
				return *plain_outer_ghosts[ghost_index];
			}
			
			//!
			inline const ghost_type &plain_inner_ghost( size_t ghost_index ) const throw() 
			{
				assert( ghost_index > 0 ); assert( ghost_index <= plain_ghosts );
				return *plain_inner_ghosts[ghost_index];
			}
			
			//!
			inline const ghost_type &async_inner_ghost( size_t ghost_index ) const throw()
			{
				assert( ghost_index > 0 ); assert( ghost_index <= async_ghosts );
				return *async_inner_ghosts[ghost_index];
			}
			
			//!
			inline const ghost_type &async_outer_ghost( size_t ghost_index ) const throw()
			{
				assert( ghost_index > 0 ); assert( ghost_index <= async_ghosts );
				return *async_outer_ghosts[ghost_index];
			}
			
			
			//! acquire memory for asynchronous ghosts
			void acquire_ghosts_data( size_t nvar )
			{
				for( size_t g=1; g <= async_ghosts; ++g )
				{
					async_inner_ghost(g).acquire_data(nvar);
					async_outer_ghost(g).acquire_data(nvar);
				}
			}
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
			vector<data_block_ptr> blocks;
			
			typedef shared_ptr<axis_type> axis_ptr;
			vector<axis_ptr> vaxis;
			axis_ptr        *axis_;
			
			vector<ghost_ptr> plain_outer_ghosts;
			vector<ghost_ptr> plain_inner_ghosts;
			vector<ghost_ptr> async_outer_ghosts;
			vector<ghost_ptr> async_inner_ghosts;
			
			//! compute outline from outer ghosts
			/**
			 TODO: check inner async only ?
			 */
			static inline layout_type compute_outline( const layout_type &L, const ghosts_type &G )
			{
				const coord_t &outer_lo = G.outer.lower.count;
				const coord_t &outer_up = G.outer.upper.count;
				const coord_t &inner_lo = G.inner.lower.count;
				const coord_t &inner_up = G.inner.upper.count;
				workspace_base::check_ghosts( &outer_lo, &outer_up, &inner_lo, &inner_up, &L.width, DIMENSIONS );
				const_coord out_lo = L.lower - outer_lo;
				const_coord out_up = L.upper + outer_up;
				return layout_type(out_lo,out_up);
			}
			
			
			static inline unit_t & __get( coord_t &coord, size_t dim ) throw()
			{
				assert(dim>=0); assert(dim<layout_type::DIMENSIONS);
				return *(((unit_t *)&coord)+dim);
			}
			
			inline void create_ghosts( const ghosts_type &G )
			{
				//-- setup has been checked in compute_outline
				const ghosts_group<coord_t> &outer_group = G.outer;
				const ghosts_group<coord_t> &inner_group = G.inner;
				
				const ghosts_infos<coord_t> &outer_lo    = outer_group.lower;
				const ghosts_infos<coord_t> &outer_up    = outer_group.upper;
				
				const ghosts_infos<coord_t> &inner_lo    = inner_group.lower;
				const ghosts_infos<coord_t> &inner_up    = inner_group.upper;
				
				const unit_t *outer_lo_count = (unit_t*) &outer_lo.count;
				const unit_t *outer_lo_async = (unit_t*) &outer_lo.async;
				const unit_t *outer_lo_peers = (unit_t*) &outer_lo.peers;
				
				const unit_t *outer_up_count = (unit_t*) &outer_up.count;
				const unit_t *outer_up_async = (unit_t*) &outer_up.async;
				const unit_t *outer_up_peers = (unit_t*) &outer_up.peers;
				
				const unit_t *inner_lo_count = (unit_t*) &inner_lo.count;
				const unit_t *inner_lo_async = (unit_t*) &inner_lo.async;
				const unit_t *inner_lo_peers = (unit_t*) &inner_lo.peers;
				
				const unit_t *inner_up_count = (unit_t*) &inner_up.count;
				const unit_t *inner_up_async = (unit_t*) &inner_up.async;
				const unit_t *inner_up_peers = (unit_t*) &inner_up.peers;
				
				coord_t       nucleus_lo( nucleus.lower);
				coord_t       nucleus_up( nucleus.upper);
				unit_t       *limit_lo = (unit_t *) & nucleus_lo;
				unit_t       *limit_up = (unit_t *) & nucleus_up;
				
				for( unsigned i=0; i < DIMENSIONS; ++i )
				{
					const size_t i2 = (i << 1);
					const ghost_position pos_lo = ghost_position(i2);
					const ghost_position pos_up = ghost_position(i2+1);
					//==========================================================
					// outer: lower then upper
					//==========================================================
					{
						//------------------------------------------------------
						// outer.lower
						//------------------------------------------------------
						{
							const unit_t ng = outer_lo_count[i];
							if( ng > 0 )
							{
								coord_t lo( this->outline.lower );
								coord_t up( this->outline.upper );
								__get(up,i)  = __get(lo,i) + (ng-1);
								const bool      async = outer_lo_async[i] != 0;
								const int       peer  = outer_lo_peers[i];
								const ghost_ptr g( new ghost_type(pos_lo,lo,up,this->outline,async,peer) );
								if( async )
								{
									async_outer_ghosts.push_back(g);
								}
								else
								{
									plain_outer_ghosts.push_back(g);
								}
							}
						}
						
						//------------------------------------------------------
						// outer.upper
						//------------------------------------------------------
						{
							const unit_t ng = outer_up_count[i];
							if( ng > 0 )
							{
								coord_t lo( this->outline.lower );
								coord_t up( this->outline.upper );
								__get(lo,i)  = __get(up,i) - (ng-1);
								const bool      async = outer_up_async[i] != 0;
								const int       peer  = outer_up_peers[i];
								const ghost_ptr g( new ghost_type(pos_up,lo,up,this->outline,async,peer) );
								if( async )
								{
									async_outer_ghosts.push_back(g);
								}
								else
								{
									plain_outer_ghosts.push_back(g);
								}
							}
						}
						
						
					}
					
					//==========================================================
					// inner : upper then lower to match outer peers 
					//==========================================================
					{
						
						//------------------------------------------------------
						// inner.upper
						//------------------------------------------------------
						{
							const unit_t ng = inner_up_count[i];
							if( ng > 0 )
							{
								coord_t lo( this->outline.lower );
								coord_t up( this->outline.upper );
								__get(up,i) -= ng;
								__get(lo,i)  = __get(up,i) - (ng-1);
								
								const bool      async = inner_up_async[i] != 0;
								const int       peer  = inner_up_peers[i];
								const ghost_ptr g( new ghost_type(pos_up,lo,up,this->outline,async,peer) );
								if( async )
								{
									async_inner_ghosts.push_back(g);
									limit_up[i] -= ng;
								}
								else
								{
									plain_inner_ghosts.push_back(g);
								}
							}
						}
						
						//------------------------------------------------------
						// inner.lower
						//------------------------------------------------------
						{
							const unit_t ng = inner_lo_count[i];
							if( ng > 0 )
							{
								coord_t lo( this->outline.lower );
								coord_t up( this->outline.upper );
								__get(lo,i) += ng;
								__get(up,i)  = __get(lo,i) + (ng-1);
								
								const bool      async = inner_lo_async[i] != 0;
								const int       peer  = inner_lo_peers[i];
								const ghost_ptr g( new ghost_type(pos_lo,lo,up,this->outline,async,peer) );
								
								if( async )
								{
									async_inner_ghosts.push_back(g);
									limit_lo[i] += ng;
								}
								else
								{
									plain_inner_ghosts.push_back(g);
								}
							}
						}
						
						
						
					}
					
					//==========================================================
					// checking user info are all right
					//==========================================================
					
					if( async_inner_ghosts.size() != async_outer_ghosts.size() )
						throw exception("mismtach async ghosts: outer=%u / inner=%u", unsigned( async_outer_ghosts.size() ), unsigned(async_inner_ghosts.size()) );
					
					if( plain_inner_ghosts.size() != plain_outer_ghosts.size() )
						throw exception("mismtach plain ghosts: outer=%u / inner=%u", unsigned( plain_outer_ghosts.size() ), unsigned(plain_inner_ghosts.size()) );
					
					for( size_t g = plain_inner_ghosts.size(); g>0; --g )
					{
						const ghost_type &g_outer = *plain_outer_ghosts[g];
						const ghost_type &g_inner = *plain_inner_ghosts[g];
						if( ghost_position_mirror(g_outer.position) != g_inner.position )
							throw exception("inner.%s sent to outer.%s", g_inner.label(), g_outer.label());
						
						if( g_outer.count != g_inner.count )
							throw exception("#outer.%s=%u != #inner.%s=%u", g_outer.label(), unsigned(g_outer.count), g_inner.label(), unsigned(g_inner.count));
					}
					
					//==========================================================
					// update parameters
					//==========================================================
					(size_t&) async_ghosts = async_outer_ghosts.size();
					(size_t&) plain_ghosts = plain_outer_ghosts.size();
					new ( (void*)&nucleus ) layout_type(nucleus_lo,nucleus_up);
				}
				
			}
			
			
		};
	}
	
}

#endif
