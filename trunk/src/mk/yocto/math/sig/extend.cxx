#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extend.hpp"

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
        
    }
}
