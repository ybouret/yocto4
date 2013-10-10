#ifndef YOCTO_SORT_NETWORK_INCLUDED
#define YOCTO_SORT_NETWORK_INCLUDED 1

#include "yocto/code/bswap.hpp"

namespace yocto
{
    
    template <typename T>
    struct netsort
    {
        static inline
        void level2( T &p0, T &p1 )
        {
            if(p1<p0) cswap<T>(p0,p1);
        }
        
        static inline
        void level3( T &p0, T &p1, T &p2 )
        {
            level2(p0, p1);
            level2(p1, p2);
            level2(p0, p1);
        }
        
        static inline
        void level4( T &p0, T &p1, T &p2, T &p3 )
        {
            level2(p0, p1);
            level2(p2, p3);
            level2(p0, p2);
            level2(p1, p3);
            level2(p1, p2);
        }
        
        static inline
        void level6( T &p0, T &p1, T &p2, T &p3, T &p4, T &p5)
        {
            level3(p0, p1, p2);
            level3(p3, p4, p5);
            level2(p0, p3);
            level2(p2, p5);
            level4(p1, p2, p3, p4);
        }
    
    };
    
}
#endif

