#ifndef YOCTO_CODE_REMOVE_IF_INCLUDED
#define YOCTO_CODE_REMOVE_IF_INCLUDED

#include "yocto/code/round.hpp"
#include <cstring>

namespace yocto
{
    
    //! use a functionoid to remove if is bad
    /**
     reject the bad values at the end of the array
     \return the number of good values
     */
    template <
    typename T,
    typename FUNC>
    inline
    size_t remove_if( T *a, size_t n, FUNC &is_bad ) throw()
    {
        assert(!(a==0&&n>0));
        size_t   n_ok = n;
        size_t   curr = 0;
        uint64_t wksp[ YOCTO_U64_FOR_ITEM(T) ];
        void    *addr = &wksp[0];
        
        while(curr<n_ok)
        {
            T *source = &a[curr];
            if( is_bad( *source) )
            {
                memmove(addr,          (void*)source,     sizeof(T)             );
                memmove((void*)source, (void*)&source[1], (--n_ok-curr)*sizeof(T) );
                memmove((void*)&a[n_ok],addr,sizeof(T));
            }
            else
                ++curr;
        }
        return n_ok;
    }
    
    
    
    template <typename ARRAY,typename FUNC> inline
    void remove_if( ARRAY &a, FUNC &is_bad) throw()
    {
        size_t n = a.size();
        if(n>0)
        {
            n = remove_if<typename ARRAY::type,FUNC>(&a[1], n, is_bad);
            while(a.size()>n) a.pop_back();
        }
    }
}


#endif
