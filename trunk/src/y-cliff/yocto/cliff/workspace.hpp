#ifndef YOCTO_CLIFF_WORKSPACE_INCLUDED
#define YOCTO_CLIFF_WORKSPACE_INCLUDED 1

#include "yocto/cliff/array1d.hpp"
#include "yocto/cliff/components.hpp"
#include "yocto/cliff/ghosts.hpp"

#include "yocto/type-traits.hpp"
#include "yocto/code/static-check.hpp"
#include "yocto/shared-ptr.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		//! utilities for workspace setup
		struct workspace_base
		{
			static void check_ghosts( const void *ghosts_lo, const void *ghosts_up, const void *w, size_t n);
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
			
			const layout_type outline; //!< original layout+ghosts
			const region_type region;  //!< real space associated to layout (NOT outline)
			const vertex_t    delta;   //!< step for each dimension
			const vertex_t    inv_d;   //!< 1/delta
			const vertex_t    inv_dsq; //!< 1/delta^2
			const layout_type nucleus; //!< original layout - deferred ghosts
			const size_t      ghosts;  //!< number of ghosts (outer,inner)
			const size_t      deferred_ghosts; //!< inner deferred ghosts
			
			//! construct a workspace
			explicit workspace(const layout_type &L,
							   const ghosts_type &G,
							   const region_type &R,
							   size_t       a,
							   size_t       b,
							   const char  *names_list[]
							   ) :
			layout_type( L ),
			components(a,b,names_list),
			outline( compute_outline( *this, G.lower.count, G.upper.count) ),
			region(R),
			delta(),
			inv_d(),
			inv_dsq(),
			nucleus( *this ),
			ghosts(0),
			deferred_ghosts(0),
			blocks( this->size, as_capacity ),
			block_(NULL),
			vaxis( DIMENSIONS, as_capacity ),
			axis_(NULL),
			outer_ghosts(),
			inner_ghosts(),
			async_ghosts()
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
					const U       *L   = (const U *     ) &(region.length);
					workspace_base::check_widths(w,DIMENSIONS);
					for( size_t i=0; i < DIMENSIONS; ++i )
					{
						const U del = (d[i]  = L[i] / ( w[i]-1));
						id[i]  = U(1)/del;
						id2[i] = U(1)/(del*del); 
					}
				}
				
				
				//--------------------------------------------------------------
				// allocate memory for each component
				//--------------------------------------------------------------
				for( size_t i=1; i <= size; ++i )
				{
					const data_block_ptr blk( new data_block( outline ) );
					blocks.push_back( blk );
				}
				block_ = &blocks[1] - this->cmin;
				
				//--------------------------------------------------------------
				// allocate memory for each axis, and compute values
				//--------------------------------------------------------------
				{
					const unit_t *lo  = (const unit_t *) & (outline.lower);
					const unit_t *hi  = (const unit_t *) & (outline.upper);
					const U      *d   = (const U *) &delta;
					const U      *o   = (const U *) &region.min;
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
			inline const axis_type & axis(size_t idim) const throw()
			{
				assert(idim<DIMENSIONS);
				return *axis_[idim];
			}
			
			//! returns an array of valid component
			inline data_block & operator[]( const size_t c ) throw()
			{
				assert(c>=this->cmin);
				assert(c<=this->cmax);
				return * block_[c];
			}
			
			//! returns an array of valid component
			inline const data_block & operator[]( const size_t c ) const throw()
			{
				assert(c>=this->cmin);
				assert(c<=this->cmax);
				return * block_[c];
			}
			
			
			//! returns an array of valid component
			inline data_block & operator[]( const string &id )
			{
				const components &comp = *this;
				return * block_[ comp(id) ];
			}
			
			//! returns an array of valid component
			inline const data_block & operator[]( const string &id ) const throw()
			{
				const components &comp = *this;
				return * block_[ comp(id) ];
			}
			
			//! returns an array of valid component
			inline data_block & operator[]( const char *id )
			{
				const components &comp = *this;
				return * block_[ comp(id) ];
			}
			
			//! returns an array of valid component
			inline const data_block & operator[]( const char *id ) const throw()
			{
				const components &comp = *this;
				return * block_[ comp(id) ];
			}
			
			//! check that the indices are valid
			inline void check_indices( const array<size_t> &cid )	
			{
				workspace_base::check_indices( cid, *this );
			}
			
			//! query and array of variables
			inline void query( array<T> &var, const array<size_t> &cid, size_t offset ) const throw()
			{
				assert( offset < this->outline.items );
				assert( var.size() >= cid.size() );
				for( size_t j = cid.size(); j > 0 ; --j )
				{
					var[j] = (*this)[ cid[j] ].entry[offset];
				}
			}
			
			//! store an array of variables
			inline void store( const array<T> &var, const array<size_t> &cid, size_t offset ) throw()
			{
				assert( offset < this->outline.items );
				assert( var.size() >= cid.size() );
				for( size_t j = cid.size(); j > 0 ; --j )
				{
					(*this)[ cid[j] ].entry[offset] = var[j];
				}
			}
			
			inline const ghost_type &outer_ghost( size_t ghost_index ) const throw() 
			{
				assert( ghost_index > 0 ); assert( ghost_index <= ghosts );
				return *outer_ghosts[ghost_index];
			}
			
			
			
			inline const ghost_type &inner_ghost( size_t ghost_index ) const throw() 
			{
				assert( ghost_index > 0 ); assert( ghost_index <= ghosts );
				return *inner_ghosts[ghost_index];
			}
			
			//! get inner deferred ghosts
			inline const ghost_type &deferred_ghost( size_t ghost_index ) const throw()
			{
				assert( ghost_index > 0 ); assert( ghost_index <= deferred_ghosts );
				return *async_ghosts[ghost_index];
			}
			
			
			//! acquire memory for deferred ghosts
			void acquire_ghosts_data( size_t nvar )
			{
				for( size_t g=1; g <= ghosts; ++g )
				{
					if( inner_ghost(g).deferred ) inner_ghost(g).acquire_data(nvar);
					if( outer_ghost(g).deferred ) outer_ghost(g).acquire_data(nvar);
				}
			}
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
			vector<data_block_ptr> blocks;
			data_block_ptr        *block_;
			
			typedef shared_ptr<axis_type> axis_ptr;
			vector<axis_ptr> vaxis;
			axis_ptr        *axis_;
			
			vector<ghost_ptr> outer_ghosts;
			vector<ghost_ptr> inner_ghosts;
			vector<ghost_ptr> async_ghosts;
			
			static inline layout_type compute_outline( const layout_type &L, param_coord ghosts_lo, param_coord ghosts_up )
			{
				workspace_base::check_ghosts( &ghosts_lo, &ghosts_up, &L.width, DIMENSIONS );
				const_coord out_lo = L.lower - ghosts_lo;
				const_coord out_up = L.upper + ghosts_up;
				return layout_type(out_lo,out_up);
			}
			
			
			static inline unit_t & __get( coord_t &coord, size_t dim ) throw()
			{
				assert(dim>=0); assert(dim<layout_type::DIMENSIONS);
				return *(((unit_t *)&coord)+dim);
			}
			
			inline void create_ghosts(const ghosts_type &G)
			{
				const ghosts_info<coord_t> &ghosts_lo = G.lower;
				const ghosts_info<coord_t> &ghosts_up = G.upper;
				
				const unit_t *glo      = (const unit_t *) &ghosts_lo.count;
				const unit_t *gup      = (const unit_t *) &ghosts_up.count;
				const unit_t *async_lo = (const unit_t *) &ghosts_lo.deferred;
				const unit_t *async_up = (const unit_t *) &ghosts_up.deferred;
				
				unit_t       *limit_lo = (unit_t *) & nucleus.lower;
				unit_t       *limit_up = (unit_t *) & nucleus.upper;
				
				for( size_t i=0; i < DIMENSIONS; ++i )
				{
					const size_t i2 = (i << 1);
					const ghost_position pos_lo = ghost_position(i2);
					const ghost_position pos_up = ghost_position(i2+1);
					const bool           deferred_lo = async_lo[i] != 0;
					const bool           deferred_up = async_up[i] != 0;
					//----------------------------------------------------------
					// lower coordinate
					//----------------------------------------------------------
					assert(glo[i]>=0); //-- checked in compute_outline
					if( glo[i] > 0 )
					{
						const unit_t    ng = glo[i];
						//-- => lower outer ghost
						{
							coord_t lo(this->lower); 
							coord_t up(this->upper); 
							
							__get(up,i)  = __get(lo,i) - 1;
							__get(lo,i) -= ng;
							
							const ghost_ptr g( new ghost_type( pos_lo,lo,up,this->outline,deferred_lo) );
							outer_ghosts.push_back( g );
						}
						
						//-- => inner upper ghost, corresponding deferred status
						{
							coord_t lo(this->lower); 
							coord_t up(this->upper); 
							
							__get(lo,i) = __get(up,i) - (ng-1);
							
							const ghost_ptr g( new ghost_type( pos_up,lo,up,this->outline,deferred_lo) );
							inner_ghosts.push_back( g );
							if( deferred_lo )
							{
								async_ghosts.push_back( g );
								limit_up[i] -= ng;
							}
						}
						
					}
					
					//----------------------------------------------------------
					// upper coordinate
					//----------------------------------------------------------
					assert(gup[i]>=0); //-- checked in compute_outline
					if( gup[i] > 0 )
					{
						const unit_t    ng = gup[i];
						//-- => upper outer ghost
						{
							coord_t lo(this->lower); 
							coord_t up(this->upper);
							
							__get(lo,i) = __get(up,i) + 1;
							__get(up,i) += ng;
							
							const ghost_ptr g( new ghost_type( pos_up,lo,up,this->outline,deferred_up) );
							outer_ghosts.push_back( g );
						}
						
						//-- => inner lower ghost, corresponding deferred status
						{
							coord_t lo(this->lower); 
							coord_t up(this->upper); 
							
							__get(up,i) = __get(lo,i) + (ng-1);
							
							const ghost_ptr g( new ghost_type( pos_lo,lo,up,this->outline,deferred_up) );
							inner_ghosts.push_back( g );
							if( deferred_up )
							{
								async_ghosts.push_back( g );
								limit_lo[i] += ng;
							}
						}
					}
				}
				
				assert( inner_ghosts.size() == outer_ghosts.size() );
				(size_t &)ghosts          = outer_ghosts.size();
				(size_t &)deferred_ghosts = async_ghosts.size(); 
			}
			
			
		};
	}
	
}

#endif
