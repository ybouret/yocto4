#include "yocto/math/alg/ellipse.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        ellipse<real_t>:: ~ellipse() throw() {}
        
        template <>
        ellipse<real_t>:: ellipse() :
        S(6,6)
        {
            
        }
        
        template <>
        void ellipse<real_t>:: reset() throw()
        {
            S.ldz();
        }
        
        template <>
        void ellipse<real_t>::append( real_t x, real_t y) throw()
        {
            const real_t  z[6] = { x*x, x*y, y*y, x, y , 1};
            const real_t *Z=z-1;
            
            for(size_t i=1; i <=6; ++i )
            {
                for(size_t j=1; j<= 6; ++j)
                    S[i][j] += Z[i] * Z[j];
            }
        }
        
        
    }
    
}
