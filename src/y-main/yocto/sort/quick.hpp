#ifndef YOCTO_SORT_QUICK_INCLUDED
#define YOCTO_SORT_QUICK_INCLUDED 1

#include "yocto/code/bswap.hpp"
#include "yocto/code/bmove.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
#define YOCTO_QSORT_MAX_LEVELS 64
    template <typename T,typename FUNC>
    inline void qsort( array<T> &arr, FUNC &compare) throw()
    {
        //----------------------------------------------------------------------
		// local memory
        //----------------------------------------------------------------------
        size_t       beg[YOCTO_QSORT_MAX_LEVELS];
        size_t       end[YOCTO_QSORT_MAX_LEVELS];
		uint64_t     tmp[YOCTO_U64_FOR_ITEM(T) ];
		T           &piv  = *_cast::trans<T,uint64_t>(tmp);
                
        //----------------------------------------------------------------------
		// initialize stacks
        //----------------------------------------------------------------------
        int  i=0;
        beg[0]=1;
        end[0]=arr.size()+1;
        
        //----------------------------------------------------------------------
		// quick sort
        //----------------------------------------------------------------------
        while(i>=0)
        {
            size_t L=beg[i];
            size_t R=end[i]-1;
            if (L<R)
            {
                bmove(piv,arr[L]);
                while (L<R)
                {
                    while (compare(piv,arr[R])<=0 && L<R) R--; if (L<R) bmove(arr[L++],arr[R]);
                    while (compare(arr[L],piv)<=0 && L<R) L++; if (L<R) bmove(arr[R--],arr[L]);
                }
                bmove(arr[L],piv);

                const int ip=i+1;
                beg[ip]=L+1;
                end[ip]=end[i];
                end[i] =L;
                
                const int im = i;
                i=ip;
                
                if(end[i]-beg[i]>end[im]-beg[im])
                {
                    cswap(beg[i],beg[im]);
                    cswap(end[i],end[im]);
                }
            }
            else
            {
                i--;
            }
        }
    }
    
    //! default sorting
	template <typename T>
	inline void qsort( array<T> &ra ) throw() { qsort( ra, __compare<T> ); }

}

#endif
