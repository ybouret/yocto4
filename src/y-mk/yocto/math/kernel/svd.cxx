#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
    
    namespace math
    {
        static inline
        real_t pythag(real_t a, real_t b)
        {
            const real_t absa=Fabs(a);
            const real_t absb=Fabs(b);
            if (absa > absb)
                return absa*Sqrt(1.0+Square(absb/absa));
            else
                return (absb <= 0.0 ? 0.0 : absb*Sqrt(1.0+Square(absa/absb)));
        }
    }
    
}