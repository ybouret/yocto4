#ifndef YOCTO_SWAMP_REGION_INCLUDED
#define YOCTO_SWAMP_REGION_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        void check_possible_mapping( unit_t lo, unit_t up, size_t idim);
        
        //! base class for region
		template <typename U>
		class region_base 
		{
		public:
			const size_t dimensions;
			virtual ~region_base() throw();
			
		protected:
			explicit region_base(size_t n) throw();
			
			//! order pmin and pmax, compute plen, return product of plen
			U        setup( const void *pmin, const void *pmax, const void *plen );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(region_base);
		};
		
		//! region of float/double
		template <typename U, template <class> class VERTEX>
		class region : public region_base<U>
		{
		public:
			typedef typename VERTEX<U>::type                     vertex;
			typedef typename type_traits<vertex>::parameter_type param_vertex;
			
			static const size_t DIMENSIONS = sizeof(vertex)/sizeof(U);
			
			const vertex vmin;    //!< minimal x[,y[,z]]
			const vertex vmax;    //!< maximal x[,y[,z]]
			const vertex length;  //!< vmax-vmin
			const U      space;   //!< product of lengths (length,area,volume)
			
			//! default region
			explicit region( param_vertex inf, param_vertex sup) : 
			region_base<U>( DIMENSIONS ),
			vmin( inf ),
			vmax( sup ),
			length(),
			space( region_base<U>::setup( &vmin, &vmax, &length ) ) 
			{
			}
			
			//! direct copy
			inline region( const region &r ) throw() :
			region_base<U>( DIMENSIONS ),
			vmin(   r.vmin   ),
			vmax(   r.vmax   ),
			length( r.length ),
			space(  r.space  ) {}
			
			
			virtual ~region() throw() {}
			
            template <typename LAYOUT>
            region map_to( const LAYOUT &target, const LAYOUT &source )
            {
                YOCTO_STATIC_CHECK(DIMENSIONS==LAYOUT::DIMENSIONS,region_dim_error);
                vertex new_vmin;
                vertex new_vmax;
                U *      pMin = (U*)&new_vmin;
                U *      pMax = (U*)&new_vmax;
                const U *qMin = (const U*)&vmin;
                const U *qLen = (const U*)&length;
                
                const unit_t *pLower = (const unit_t *)& target.lower;
                const unit_t *pUpper = (const unit_t *)& target.upper;
                
                const unit_t *qLower = (const unit_t *)& source.lower;
                const unit_t *qUpper = (const unit_t *)& source.upper;
                
                for( size_t i=0; i < DIMENSIONS; ++i )
                {
                    const unit_t lo = qLower[i];
                    const unit_t up = qUpper[i];
                    check_possible_mapping(lo,up,i);
                    const unit_t del = up - lo;
                    const U      len = qLen[i];
                    const U      org = qMin[i];
                    
                    pMin[i] = org + ( len * ( pLower[i] - lo ) ) / del;
                    pMax[i] = org + ( len * ( pUpper[i] - lo ) ) / del;
                }
                
                return region( new_vmin, new_vmax );
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(region);
        };
        
    }
}

#endif
