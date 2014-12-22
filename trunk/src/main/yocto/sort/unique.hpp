#ifndef YOCTO_CODE_UNIQUE_INCLUDED
#define YOCTO_CODE_UNIQUE_INCLUDED 1

#include "yocto/sort/heap.hpp"
#include <cstring>

namespace yocto
{
    
    
    //! rearrange an array of ORDERED values
    /**
     \return the number of unique items
     */
    template <typename T,typename FUNC>
    inline size_t __unique( T *a, const size_t n, FUNC &compare) throw()
    {
        assert(!(a==0&&n>0));
        if(n<=1)
        {
            return n;
        }
        else
        {
            assert(n>=2);
            //-- local memory
            uint64_t     wksp[ YOCTO_U64_FOR_ITEM(T) ];
            T           *ra   = _cast::trans<T,uint64_t>(wksp);
            
            //-- initialize algorithm
            size_t       n_ok = n;
            
            size_t curr = 0;
            size_t next = 1;
            while(next<n_ok)
            {
                assert(compare(a[curr],a[next]) <= 0);
                
                if( compare(a[curr],a[next]) == 0 )
                {
                    T *target = &a[next];
                    memmove((void*)ra,        (void*)target,     sizeof(T));
                    memmove((void*)target,    (void*)(target+1), (n_ok-next)*sizeof(T));
                    memmove((void*)&a[--n_ok],(void*)ra,         sizeof(T));
                }
                else
                {
                    ++next;
                    ++curr;
                }
            }
            return n_ok;
        }
    }
    
    
    //! rearrange an array of values
    /**
     \return the number of unique items
     */
    template <typename T,typename FUNC>
    inline size_t unique( T *a, const size_t n, FUNC &compare) throw()
    {
        hsort<T>(a, n, compare);
        return __unique<T>(a, n, compare);
    }
    
    //! sort and cleanup the array
    /**
     assume the array is not a list...
     */
    template <typename ARRAY,typename FUNC>
    inline void unique( ARRAY &a, FUNC &compare) throw()
    {
        const size_t n = a.size();
        if( n > 1 )
        {
            const size_t n_ok = unique<typename ARRAY::type>( &a[1], n, compare);
            while( a.size() > n_ok )
                a.pop_back();
        }
    }
    
    
}

#endif
