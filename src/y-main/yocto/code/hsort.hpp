#ifndef YOCTO_HSORT_INCLUDED
#define YOCTO_HSORT_INCLUDED 1

#include "yocto/sequence/array.hpp"
#include "yocto/code/swap.hpp"

namespace yocto
{
	
	//! heap sort
	template <typename T,typename FUNC>
	void hsort( array<T> &ra, FUNC &compare ) throw()
	{
		const size_t n = ra.size();
		if (n < 2) return;
		
		//-- local memory
		uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
		void        *addr = static_cast<void*>(wksp);
		T           &rra  = *static_cast<T *>(addr);
		
		// algorithm
		size_t l =(n >> 1)+1;
		size_t ir=n;
		for (;;)
		{
			if (l>1) 
			{
				mmove(rra,ra[--l]);
			}
			else
			{
				mmove( rra,    ra[ir] );
				mmove( ra[ir], ra[1]  );
				if (--ir == 1) 
				{
					mmove(ra[1],rra);
					break;
				}
			}
			size_t i=l;
			size_t j=l+l;
			while (j <= ir) 
			{
				if (j < ir && compare(ra[j],ra[j+1]) < 0 )
					j++;
				if ( compare(rra,ra[j]) < 0) 
				{ 
					mmove( ra[i], ra[j] );
					i=j;
					j <<= 1;
				} 
				else 
					j=ir+1; 
			}
			mmove( ra[i], rra );
		}
	}
	
	//! heap co sort
	template <typename T, typename U, typename FUNC>
	void hsort( array<T> &ra, array<U> &rb, FUNC &compare ) throw()
	{
		const size_t n = ra.size(); assert( ra.size() == rb.size() );
		if (n < 2) return;
		
		//-- local memory
		uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
		void        *addr = static_cast<void*>(wksp);
		T           &rra  = *static_cast<T *>(addr);
		uint64_t     wksp2[ YOCTO_U64_FOR_ITEM(U) ];
		void        *addr2 = static_cast<void*>(wksp2);
		U           &rrb  = *static_cast<U *>(addr2);
		
		//-- algorithm
		size_t l =(n >> 1)+1;
		size_t ir=n;
		for (;;)
		{
			if (l>1) 
			{
				--l;
				mmove(rra,ra[l]);
				mmove(rrb,rb[l]);
			}
			else
			{
				mmove( rra,    ra[ir] );
				mmove( ra[ir], ra[1]  );
				
				mmove( rrb,    rb[ir] );
				mmove( rb[ir], rb[1]  );
				
				if (--ir == 1) 
				{
					mmove(ra[1],rra);
					mmove(rb[1],rrb);
					break;
				}
			}
			size_t i=l;
			size_t j=l+l;
			while (j <= ir) 
			{
				if (j < ir && compare(ra[j],ra[j+1]) < 0 )
					j++;
				if ( compare(rra,ra[j]) < 0) 
				{ 
					mmove( ra[i], ra[j] );
					mmove( rb[i], rb[j] );
					i=j;
					j <<= 1;
				} 
				else 
					j=ir+1; 
			}
			mmove( ra[i], rra );
			mmove( rb[i], rrb );
		}
	}
	
	
}

#endif
