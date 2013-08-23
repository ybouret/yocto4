#ifndef YOCTO_SORT_NETWORK_INCLUDED
#define YOCTO_SORT_NETWORK_INCLUDED 1

#include "yocto/code/swap.hpp"

namespace yocto
{
    
    template <typename T>
    inline void netsort2( T &p0, T &p1 )
    {
        if(p1<p0) cswap<T>(p0,p1);
    }
    
    template <typename T>
    inline void netsort3( T &p0, T &p1, T &p2 )
    {
        netsort2(p0, p1);
        netsort2(p1, p2);
        netsort2(p0, p1);
    }
    
    template <typename T>
    inline void netsort4( T &p0, T &p1, T &p2, T &p3 )
    {
        netsort2(p0, p1);
        netsort2(p2, p3);
        netsort2(p0, p2);
        netsort2(p1, p3);
        netsort2(p1, p2);
    }
    
    template <typename T>
    inline void netsort6( T &p0, T &p1, T &p2, T &p3, T &p4, T &p5)
    {
        netsort3(p0, p1, p2);
        netsort3(p3, p4, p5);
        netsort2(p0, p3);
        netsort2(p2, p5);
        netsort4(p1, p2, p3, p4);
    }
    
}
#endif

