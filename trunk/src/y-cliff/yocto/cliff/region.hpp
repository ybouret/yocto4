#ifndef YOCTO_CLIFF_REGION_INCLUDED
#define YOCTO_CLIFF_REGION_INCLUDED 1


#include "yocto/cliff/types.hpp"
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
			typedef typename VERTEX<U>::type                     vertex;
			typedef typename type_traits<vertex>::parameter_type param_vertex;
			
			static const size_t DIMENSIONS = sizeof(vertex)/sizeof(U);
			
			const vertex min;
			const vertex max;
			const vertex length;
			const U      space;   //!< product of lengths
			
			explicit region( param_vertex inf, param_vertex sup) : 
			region_base<U>( DIMENSIONS ),
			min( inf ),
			max( sup ),
			length(),
			space( region_base<U>::setup( &min, &max, &length ) ) 
			{
			}
			
			virtual ~region() throw()
			{
			}
			
			
		private:
			YOCTO_DISABLE_ASSIGN(region);
		};
		
	}
}

#endif
