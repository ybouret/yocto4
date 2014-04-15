#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extender.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace math
    {
        
        template <>
        extender<real_t>:: ~extender() throw() {}
        
        template <>
        extender<real_t>:: extender( extension_type lo, extension_type up) :
        lower(lo),
        upper(up)
        {
            const bool valid = (extend_cyclic==lower&&extend_cyclic==upper) || (extend_cyclic!=lower&&extend_cyclic!=upper);
            if(!valid)
                throw exception("invalid extensions type: both or none must be cyclic!");
        }
        
        template <>
        extender<real_t>:: extender( extension_type both ) throw() :
        lower(both),
        upper(both)
        {
        }
        
        template <>
        real_t extender<real_t>:: get_x(ptrdiff_t i, const array<real_t> &X, const ptrdiff_t N) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                        
                    case extend_cyclic:
                        return get_x(i+N-1,X,N) - (X[N]-X[1]);
                        
                    default:
                        return X[1]+(X[1] - get_x(2-i, X, N));
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:
                            return get_x(i-N+1,X,N)+(X[N]-X[1]);
                            
                        default:
                            return X[N] + (X[N] - get_x(N+N-i,X,N));
                    }
                }
                else
                    return X[i];
            }
            
        }
        
        template <>
        real_t extender<real_t>:: get_y( ptrdiff_t i, const array<real_t> &Y, const ptrdiff_t N ) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                    case extend_zero:
                        return 0;
                        
                    case extend_constant:
                        return Y[1];
                        
                    case extend_cyclic:
                        return get_y(i+N-1,Y,N);
                        
                    case extend_odd:
                        return Y[1] - (get_y(2-i,Y,N)-Y[1]);
                        
                    case extend_even:
                        return get_y(2-i,Y,N);
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_zero:
                            return 0;
                            
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return get_y(i-N+1,Y,N);
                            
                        case extend_odd:
                            return Y[N] - (get_y(N+N-i,Y,N)-Y[N]);
                            
                        case extend_even:
                            return get_y(N+N-i,Y,N);
                            
                    }
                }
                else
                {
                    return Y[i];
                }
            }
            return 0; // never get here
        }
        
        
        
    }
}