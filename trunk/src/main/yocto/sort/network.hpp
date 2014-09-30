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
        
        template <typename U>
        static inline
        void co_level2( T &p0, T &p1, U &q0, U &q1 )
        {
            if(p1<p0)
            {
                cswap<T>(p0,p1);
                cswap<U>(q0,q1);
            }
        }
        
        static inline
        void level2( T *p )
        {
            assert(p);
            level2(p[0],p[1]);
        }
        
        template <typename U>
        static inline void
        co_level2( T *p, U *q )
        {
            assert(p);
            assert(q);
            co_level2(p[0], p[1], q[0], q[1]);
        }
        
        
        
        static inline
        void level3( T &p0, T &p1, T &p2 )
        {
            level2(p0, p1);
            level2(p1, p2);
            level2(p0, p1);
        }
        
        template <typename U>
        static inline
        void co_level3( T &p0, T &p1, T &p2, U &q0, U &q1, U &q2 )
        {
            co_level2(p0,p1,q0,q1);
            co_level2(p1,p2,q1,q2);
            co_level2(p0,p1,q0,q1);
        }

        
        static inline
        void level3( T *p )
        {
            level3(p[0],p[1],p[2]);
        }
        
        template <typename U>
        static inline void
        co_level3( T *p, U *q )
        {
            assert(p);
            assert(q);
            co_level3(p[0], p[1], p[2], q[0], q[1], q[2]);
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

