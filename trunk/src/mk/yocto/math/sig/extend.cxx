#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extend.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace math
    {
        
        template <> extend<real_t>:: ~extend() throw() {}
        
        template <> extend<real_t>:: extend(extend_mode lo,
                                            extend_mode up) throw() :
        lower(lo),
        upper(up)
        {}
        
#if 0
        template <>
        real_t extend<real_t>:: operator()(real_t               x,
                                           const array<real_t> &X,
                                           const array<real_t> &Y) const throw()
        {
            assert(X.size()==Y.size());
            assert(X.size()>=2);
            const extend &self = *this;
            const size_t N = X.size();
            const real_t X1 = X[1];
            const real_t XN = X[N];
            if(x <= X1 )
            {
                switch(lower)
                {
                    case extend_constant:
                        return Y[1];
                        
                        
                    case extend_cyclic:
                        return self(x+(XN-X1),X,Y);
                }
            }
            else
            {
                if(x>=XN)
                {
                    switch(upper)
                    {
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return self( x-(XN-X1),X,Y);
                    }
                }
                else
                {
                    size_t klo = 1, khi = N;
					while( (khi-klo)> 1 )
					{
						const size_t k = (klo+khi)>>1;
						if( X[k] > x )
							khi = k;
						else
							klo = k;
					}
                    return Y[klo] + (x-X[klo]) * (Y[khi] - Y[klo]) / (X[khi] - X[klo]);
                    
                }
            }
            return 0;
        }
#endif
        
        template <>
        real_t extend<real_t>:: get_x( ptrdiff_t i, const array<real_t> &X, const ptrdiff_t N, const real_t L ) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                        
                    case extend_cyclic:
                    {
                        real_t dx = 0;
                        while(i<1)
                        {
                            i  += N;
                            dx += L;
                        }
                        assert(i<=N);
                        return X[i] - dx;
                    }
                        
                    default:
                        return X[1]+(X[1] - get_x(2-i, X, N, L));
                }
            }
            else
            {
                if(i>N)
                {
                    switch(upper)
                    {
                        case extend_cyclic:
                        {
                            real_t dx =0;
                            while(i>N)
                            {
                                i -=N;
                                dx+=L;
                            }
                            assert(i>=1);
                            return X[i] + dx;
                        }
                            
                        default:
                            return X[N] + (X[N] - get_x(N+N-i,X,N,L));
                    }
                }
                else
                    return X[i];
            }
            
        }
        
        template <>
        real_t extend<real_t>:: get_y( ptrdiff_t i, const array<real_t> &Y, const ptrdiff_t N ) const throw()
        {
            if(i<1)
            {
                switch(lower)
                {
                    case extend_constant:
                        return Y[1];
                        
                    case extend_cyclic:
                        return get_y(i+N,Y,N);
                        
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
                        case extend_constant:
                            return Y[N];
                            
                        case extend_cyclic:
                            return get_y(i-N,Y,N);
                            
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
        }
        
        
        template <>
        v2d<real_t> extend<real_t>:: at( ptrdiff_t i, const array<real_t> &X, const array<real_t> &Y) const
        {
            
            assert(X.size()>0);
            assert(Y.size()>0);
            assert(X.size()==Y.size());
            const ptrdiff_t N = ptrdiff_t(X.size());
            
            if(N<=0)
                throw libc::exception( ERANGE, "integer overflow in extend()");
            const real_t L = X[N] - X[1];
            v2d<real_t> ans;
            ans.x = get_x(i, X, N, L);
            ans.y = get_y(i, Y, N );
            
            return ans;
        }
        
        
        
    }
}
