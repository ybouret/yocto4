#ifndef YOCTO_CODE_ARITH_INCLUDED
#define YOCTO_CODE_ARITH_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    //! Greated Common Divider
    /**
     assuming unsigned values
     */
    template <typename T>
    inline T gcd_of( T x , T y )
    {
        if( x > 0 && y > 0 )
        {
            T a = x;
            T b = y;
            if( b > a )
            {
                a = y;
                b = x;
            }
            while( b > 0 )
            {
                const T t = b;
                b = a % b;
                a = t;
            }
            return a;
        }
        else
            return 1;
    }
    
    template <typename T>
    inline void u_simplify( T &num, T &den )
    {
        const T g = gcd_of(num,den);
        num/=g;
        den/=g;
    }
    
    template <typename T>
    inline void i_simplify( T &num, T &den )
    {
        const T anum = (num<0) ? -num : num;
        const T aden = (den<0) ? -den : den;
        const T g = gcd_of(anum,aden);
        num/=g;
        den/=g;
    }

    
    
    
    //! Least Common Multiplier
    template <typename T>
    inline T lcm_of( T x , T y )
    {
        return ( x * y ) / gcd_of<T>( x, y );
    }
    

}

#endif
