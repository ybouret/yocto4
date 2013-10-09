#ifndef YOCTO_SORT_HEAP_INCLUDED
#define YOCTO_SORT_HEAP_INCLUDED 1

#include "yocto/sequence/lw-array.hpp"
#include "yocto/code/bmove.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
	
	//! heap sort
	template <typename T,typename FUNC>
	inline void hsort( array<T> &ra, FUNC &compare ) throw()
	{
		const size_t n = ra.size();
		if (n < 2) return;
		
        //----------------------------------------------------------------------
		// local memory
        //----------------------------------------------------------------------
		uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
		T           &rra  = *_cast::trans<T,uint64_t>(wksp);
		
        //----------------------------------------------------------------------
		// algorithm
        //----------------------------------------------------------------------
		size_t l =(n >> 1)+1;
		size_t ir=n;
		for (;;)
		{
			if (l>1) 
			{
				bmove(rra,ra[--l]);
			}
			else
			{
				bmove( rra,    ra[ir] );
				bmove( ra[ir], ra[1]  );
				if (--ir == 1) 
				{
					bmove(ra[1],rra);
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
					bmove( ra[i], ra[j] );
					i=j;
					j <<= 1;
				} 
				else 
					j=ir+1; 
			}
			bmove( ra[i], rra );
		}
	}
	
	//! heap co sort
	template <typename T, typename U, typename FUNC>
	inline void hsort( array<T> &ra, array<U> &rb, FUNC &compare ) throw()
	{
		const size_t n = ra.size(); assert( ra.size() == rb.size() );
		if (n < 2) return;
		
        //----------------------------------------------------------------------
		//-- local memory
        //----------------------------------------------------------------------
		uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
		T           &rra  = *_cast::trans<T,uint64_t>(wksp);
        
		uint64_t     wksp2[ YOCTO_U64_FOR_ITEM(U) ];
		U           &rrb  = *_cast::trans<U,uint64_t>(wksp2);
		
        //----------------------------------------------------------------------
		//-- algorithm
        //----------------------------------------------------------------------
		size_t l =(n >> 1)+1;
		size_t ir=n;
		for (;;)
		{
			if (l>1) 
			{
				--l;
				bmove(rra,ra[l]);
				bmove(rrb,rb[l]);
			}
			else
			{
				bmove( rra,    ra[ir] );
				bmove( ra[ir], ra[1]  );
				
				bmove( rrb,    rb[ir] );
				bmove( rb[ir], rb[1]  );
				
				if (--ir == 1) 
				{
					bmove(ra[1],rra);
					bmove(rb[1],rrb);
					break;
				}
			}
			size_t i=l;
			size_t j=l+l;
			while (j <= ir) 
			{
				if(j < ir && compare(ra[j],ra[j+1]) < 0 )
					j++;
				if( compare(rra,ra[j]) < 0) 
				{ 
					bmove( ra[i], ra[j] );
					bmove( rb[i], rb[j] );
					i=j;
					j <<= 1;
				} 
				else 
					j=ir+1; 
			}
			bmove( ra[i], rra );
			bmove( rb[i], rrb );
		}
	}
	
    //! default sorting
	template <typename T>
	inline void hsort( array<T> &ra ) throw() { hsort( ra, __compare<T> ); }
	
    //! default co-sorting
    template <typename T, typename U>
	inline void co_hsort( array<T> &ra, array<U> &rb ) throw() { hsort( ra, rb, __compare<T> ); }
	
    //! C-style sorting
    template<typename T,typename FUNC>
    inline void hsort( T *a, size_t n, FUNC &compare )
    {
        lw_array<T> A(a,n);
        hsort<T,FUNC>(A,compare);
    }
    
    //! C-style co-sorting
    template<typename T,typename U,typename FUNC>
    inline void co_hsort( T *a, U *b, size_t n, FUNC &compare )
    {
        lw_array<T> A(a,n);
        lw_array<U> B(b,n);
        hsort<T,U,FUNC>(A,B,compare);
    }
    
}

#endif
