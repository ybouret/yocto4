#ifndef YOCTO_CODE_UNIQUE_INCLUDED
#define YOCTO_CODE_UNIQUE_INCLUDED 1

#include "yocto/code/hsort.hpp"

namespace yocto
{
    
    
    //! rearrange an array or ORDERED values
    /**
     \return the number of doublons
     */
    template <typename T,typename FUNC>
    inline size_t __unique( T *a, const size_t n, FUNC &compare) throw()
    {
        assert(!(a==0&&n>0));
        size_t ans = 0;
        if(n>1)
        {
            //-- local memory
            uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
            T           *ra   = _cast::trans<T,uint64_t>(wksp);
            size_t       n_ok = n;
            
            size_t curr = 0;
            size_t next = 1;
            while(next<n_ok)
            {
                if( compare(a[curr],a[next]) == 0 )
                {
                    T *target = &a[next];
                    memmove(ra,target,sizeof(T));
                    memmove(target,target+1,(n_ok-next)*sizeof(T));
                    memmove(&a[--n_ok],ra,sizeof(T));
                    ++ans;
                }
                else
                {
                    ++next;
                    ++curr;
                }
            }
            
        }
        return ans;
    }
    
    
    //! rearrange an array of values
    /**
     \return the number of doublons
     */
    template <typename T,typename FUNC>
    inline size_t unique( T *a, const size_t n, FUNC &compare) throw()
    {
        hsort<T>(a, n, compare);
        return __unique<T>(a, n, compare);
    }
    
    template <typename ARRAY,typename FUNC>
    inline void unique( ARRAY &a, FUNC &compare) throw()
    {
        const size_t n = a.size();
        if( n > 1 )
        {
            size_t k = unique<typename ARRAY::type>( &a[1], n, compare);
            while(k-->0)
                a.pop_back();
        }
        
    }
    
    
}

#endif
