#ifndef YOCTO_SPADE_LAYOUT_INCLUDED
#define YOCTO_SPADE_LAYOUT_INCLUDED 1

#include "yocto/spade/types.hpp"
#include "yocto/type-traits.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		//!< layout of coordinate COORD
		template <typename COORD>
		class layout : public layout_base
		{
		public:
			typedef COORD                                        coord_type;
			typedef typename type_traits<COORD>::mutable_type    mutable_coord;
			typedef const mutable_coord                          const_coord;
			typedef typename type_traits<COORD>::parameter_type  param_coord;
			
			static const size_t dims = sizeof(COORD)/sizeof(unit_t);
			
			const_coord   lower; //!< lower coordinate
			const_coord   upper; //!< upper coordinate
			const_coord   width; //!< coordinate width
			const size_t  items; //!< number of linear items within the layout = items stride
			
			
			//! prepare layout
			explicit layout( param_coord a_lower, param_coord a_upper ) throw() :
			lower( a_lower ),
			upper( a_upper ),
			width( ),
			items( layout_base::order( &lower, &upper, dims, &width ) )
			{
				
			}
			
			//! copy layout
			layout( const layout &other ) throw() :
			lower( other.lower ),
			upper( other.upper ),
			width( other.width ),
			items( other.items )
			{
			}
			
			//! MPI style splitting
			/**
			 \param world_size > 0
			 \param world_rank < world_size
			 \param idim in [0..dims-1]
			 */
			inline layout split( size_t world_size, size_t world_rank, size_t idim ) const throw()
			{
				
				assert( world_size > 0 );
				assert( world_rank < world_size );
				assert( idim < dims );
				
				//-- copy current coordinates
				coord_type s_lower = lower;
				coord_type s_upper = upper;
				
				//-- get coordinate of interest
				unit_t *L = ( (unit_t *) & s_lower ) + idim;
				unit_t *U = ( (unit_t *) & s_upper ) + idim;
				
				//-- split algorithm
				layout_base::split1( *L, *U, world_size, world_rank );
				
				//-- make a new layout
				return layout( s_lower, s_upper );
			}
			
			inline layout ghosts( size_t prev, size_t next, size_t idim ) const throw()
			{
				//-- copy current coordinates
				coord_type s_lower = lower;
				coord_type s_upper = upper;
				
				//-- get coordinate of interest
				unit_t *L = ( (unit_t *) & s_lower ) + idim;
				unit_t *U = ( (unit_t *) & s_upper ) + idim;
				*L -= prev;
				*U += next;
				
				return layout( s_lower, s_upper );
			}
			
			inline layout inside( ) const throw()
			{
				//-- copy current coordinates
				coord_type s_lower = lower;
				coord_type s_upper = upper;
				unit_t *L = ( (unit_t *) & s_lower );
				unit_t *U = ( (unit_t *) & s_upper );
				for( size_t i=0; i < dims; ++i )
				{
					L[i] += 1;
					U[i] -= 1;
				}
				
				return layout( s_lower, s_upper );
			}
			
			inline bool has( param_coord coord ) const throw()
			{
				const unit_t *p  = (const unit_t *) &coord;
				const unit_t *lo = (const unit_t *) &lower;
				const unit_t *hi = (const unit_t *) &upper;
				for( size_t i=0; i < dims; ++i )
				{
					const unit_t q = p[i];
					if( q < lo[i] || q > hi[i] )
						return false;
				}
				return true;
			}
			
			virtual ~layout() throw() {}
			
			inline unit_t offset_of( param_coord coord ) const throw()
			{
				const unit_t *p  = (const unit_t *) &coord;
				const unit_t *l  = (const unit_t *) &lower;
				const unit_t *w  = (const unit_t *) &width;
				unit_t  ans = p[0] - l[0];
				for( size_t i=1; i < dims; ++i )
				{
					unit_t tmp = p[i] - l[i];
					for( size_t j=i-1; j < i; ++j ) tmp *= w[j];
					ans += tmp;
				}
				return ans;
			}
						
			
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
		
		
	}
}

#endif
