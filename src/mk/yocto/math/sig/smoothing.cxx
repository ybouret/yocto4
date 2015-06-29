#include "yocto/math/sig/smoothing.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {

        template <>
        smoothing<real_t>:: ~smoothing() throw()
        {
        }

        template<>
        smoothing<real_t>:: smoothing() throw() :
        f(0),
        b(0),
        degree(0),
        v(),
        nf(0),
        nb(0)
        {

        }

        template <>
        void smoothing<real_t>::reset() throw()
        {
            v.free();
            nf = nb = 0;
            f  = Fabs(f);
            b  = Fabs(b);
        }



        template <>
        void smoothing<real_t>:: build(const array<real_t> &X,
                                       const array<real_t> &Y)
        {
            assert(X.size()==Y.size());
            const size_t N = X.size();
            reset();
            if(N<=0)
                return;
            try
            {
                // counting point to add to X1
                {
                    const real_t X1 = X[1];
                    for(size_t i=2;i<=N;++i)
                    {
                        const real_t dX = X[i] - X1;
                        if(dX>b)
                            break;
                        ++nb;
                    }
                }

                {
                    const real_t XN = X[N];
                    for(size_t i=N-1;i>0;--i)
                    {
                        const real_t dX = XN - X[i];
                        if(dX>f)
                            break;
                        ++nf;
                    }
                }



            }
            catch(...)
            {
                reset();
                throw;
            }
        }
        
        
    }
    
}
