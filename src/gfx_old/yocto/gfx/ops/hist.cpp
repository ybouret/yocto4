#include "yocto/gfx/ops/hist.hpp"

namespace yocto
{
    namespace gfx
    {
        
        namespace
        {
            static inline
            double get_full_variance(const double *count, size_t t)
            {
                assert(t<histogram::bins);
                
                // background average
                size_t nb   = 0;
                double mb   = 0;
                for(size_t i=0;i<t;++i)
                {
                    const size_t n = size_t(count[i]);
                    nb += n;
                    mb += double(i) * n;
                }
                
                // background variance
                double vb = 0;
                if(nb>0)
                {
                    mb /= nb;
                    for(size_t i=0;i<t;++i)
                    {
                        const double d = double(i) - mb;
                        vb += count[i] * d*d;
                    }
                    vb /= nb;
                }
                
                
                // foreground average
                size_t nf = 0;
                double mf = 0;
                for(size_t i=t;i<histogram::bins;++i)
                {
                    const size_t n = size_t(count[i]);
                    nf += n;
                    mf += double(i) * n;
                }
                
                // foreground variance
                double vf = 0;
                if(nf>0)
                {
                    mf /= nf;
                    for(size_t i=t;i<histogram::bins;++i)
                    {
                        const double d = double(i) - mf;
                        vf += count[i] * d*d;
                    }
                    vf /= nf;
                }
                
                const size_t ntot = nb + nf;
                if(ntot>0)
                {
                    return (nb*vb+nf*vf)/ntot;
                }
                else
                {
                    return 0;
                }
            }
            
        }
        
        size_t histogram:: threshold() const throw()
        {
            size_t ans = 0;
            double opt = get_full_variance(count,ans);
            for(size_t i=1;i<bins;++i)
            {
                const double tmp = get_full_variance(count,i);
                if(tmp<opt)
                {
                    ans = i;
                    opt = tmp;
                }
            }
            return ans;
        }
        
    }
}
