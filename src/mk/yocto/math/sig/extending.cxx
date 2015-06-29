#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/extending.hpp"

namespace yocto
{
    namespace math
    {

        template<>
        extension<real_t>:: extension(const extension_mode lo, const extension_mode up) throw() :
        lower(lo),
        upper(up)
        {
        }

        template<>
        real_t extension<real_t>:: get_x(const ptrdiff_t i, const array<real_t> &X, const ptrdiff_t N) const
        {
            assert(size_t(N)==X.size());
            if(i<1)
            {
                const ptrdiff_t di = 1-i;
                const ptrdiff_t ii = 1+di;
                if(ii<=N)
                {
                    
                }
                else
                {
                    return get_x(ii,X,N);
                }
            }
            else
            {
                if(i>N)
                {
                    const ptrdiff_t di = i-N;
                    const ptrdiff_t ii = N - di;
                    if(ii>=1)
                    {

                    }
                    else
                    {
                        return get_x(ii,X,N);
                    }
                }
                else
                {
                    return X[i];
                }
            }
        }
        
        
    }
    
}
