#ifndef YOCTO_CLIFF_REGION_INCLUDED
#define YOCTO_CLIFF_REGION_INCLUDED 1


#include "yocto/cliff/layout.hpp"
#include "yocto/type-traits.hpp"

namespace yocto
{
	namespace cliff
	{
		
		template <typename U>
		class region_base 
		{
		public:
			const size_t dimensions;
			virtual ~region_base() throw();
			
		protected:
			explicit region_base(const size_t n) throw();
			//! order pmin and pmax, compute plen, return product of plen
			U        setup( const void *pmin, const void *pmax, const void *plen );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(region_base);
		};
		
		template <typename U, template <class> class VERTEX>
		class region : public region_base<U>
		{
		public:
			typedef typename VERTEX<U>::type                       vertex_t;
			typedef typename type_traits<vertex_t>::parameter_type param_vertex;
			
			static const size_t DIMENSIONS = sizeof(vertex_t)/sizeof(U);
			
			const vertex_t min;
			const vertex_t max;
			const vertex_t length;
			const U        space;   //!< product of lengths (length,area,volume)
			
			//! default region
			explicit region( param_vertex inf, param_vertex sup) : 
			region_base<U>( DIMENSIONS ),
			min( inf ),
			max( sup ),
			length(),
			space( region_base<U>::setup( &min, &max, &length ) ) 
			{
			}
			
			//! direct copy
			region( const region &r ) throw() :
			region_base<U>( DIMENSIONS ),
			min( r.min ),
			max( r.max ),
			length( r.length ),
			space(  r.space  )
			{
			}
			
			
			virtual ~region() throw()
			{
			}
			
			template <typename LAYOUT>
			static region extract( const region &r, const LAYOUT &master, const LAYOUT &sub ) throw()
			{
				assert( master.has( sub.lower ) );
				assert( master.has( sub.upper ) );
				
				vertex_t      sub_min, sub_max;
				const U      *region_min    = (U *) &(r.min);
				const U      *region_length = (U *) &(r.length);
				const unit_t *master_lower  = (const unit_t *) &(master.lower);
				const unit_t *master_width  = (const unit_t *) &(master.width);
				const unit_t *sub_lower     = (const unit_t *) &(sub.lower);
				const unit_t *sub_upper     = (const unit_t *) &(sub.upper);
				U *           q_min         = (U*) &sub_min;
				U *           q_max         = (U*) &sub_max;
				for( size_t i=0; i < DIMENSIONS; ++i )
				{
					const U      num  = region_length[i]; 
					const unit_t den  = master_width[i]-1; 
					const U      vini = region_min[i]; 
					const unit_t uini = master_lower[i];
					
					q_min[i] = vini + (num*(sub_lower[i] - uini))/den;
					q_max[i] = vini + (num*(sub_upper[i] - uini))/den;
				}
				return region( sub_min, sub_max);
			}
			
		private:
			YOCTO_DISABLE_ASSIGN(region);
		};
		
		
		
	}
}

#endif
