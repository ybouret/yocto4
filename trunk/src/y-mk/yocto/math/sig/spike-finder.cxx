
#include "yocto/math/sig/spike-finder.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {
        
        
        namespace
        {
            static inline
            bool __find_spike( spike_info<real_t> &spike,
                              const array<real_t> &x,
                              const array<real_t> &y,
                              const array<bool>   &used) throw()
            {
                assert( x.size() == y.size() );
                assert( x.size() == used.size() );
                const size_t n = x.size();
                for( size_t i=1; i <= n; ++i )
                {
                    if( !used[i] )
                    {
                        //-- initialize
                        spike.idx = i;
                        spike.pos = x[i];
                        spike.val = y[i];
                        
                        //-- find next
                        for(size_t j=i+1;j<=n;++j)
                        {
                            if( !used[j] )
                            {
                                if( y[j] > spike.val )
                                {
                                    spike.idx = j;
                                    spike.pos = x[j];
                                    spike.val = y[j];
                                }
                            }
                        }
                        return true;
                    }
                }
                //-- no usable data
                return false;
            }
            
            static inline
            void __compute_max( spike_info<real_t> &spike,
                               const array<real_t> &x,
                               const array<real_t> &y )
            {
                assert( x.size() == y.size() );
                const size_t n = x.size();
                assert(spike.idx>=1);
                assert(spike.idx<=n);
                if( spike.idx > 1 && spike.idx < n )
                {
                    const real_t x0 = x[spike.idx-1];
                    const real_t x1 = x[spike.idx]-x0;
                    const real_t x2 = x[spike.idx+1]-x0;
                    if( x2 > 0 || x2 < 0 )
                    {
                        const real_t alpha = x1/x2;
                        const real_t f0    = y[spike.idx-1];
                        const real_t beta  = y[spike.idx]   - f0;
                        const real_t gam   = y[spike.idx+1] - f0; assert(gam<=beta);
                        if(beta>0)
                        {
                            const real_t X = clamp<real_t>(0,REAL(0.5) * (beta - alpha*alpha*gam)/(beta-alpha*gam),1);
                            //std::cerr << "X=" << X << std::endl;
                            spike.pos = x0 + X * x2;
                            
                        }
                    }
                    
                }
                // else do nothing => global max on the side !
            }
        }
        
        template <>
        size_t spike_finder<real_t>:: detect(sequence< spike_info<real_t> > &spikes,
                                             const size_t                    count,
                                             const array<real_t>            &x,
                                             const array<real_t>            &y)
        {
            assert( x.size() == y.size() );
            const size_t n = x.size();
            
            //==================================================================
            //-- initially, no data is used
            //==================================================================
            vector<bool> used(n,false);
            
            for( size_t iter=1;;++iter)
            {
                spike_info<real_t> spike;
                
                //==============================================================
                // find the next maximum
                //==============================================================
                if( !__find_spike(spike, x, y, used) )
                {
                    return (iter-1);
                }
                
                //==============================================================
                // compute the second order maximum
                //==============================================================
                __compute_max(spike, x, y);
                
                spikes.push_back(spike);
                if(count>0 && iter>=count)
                {
                    return iter;
                }
                
                //==============================================================
                // tag the data
                //==============================================================
                used[spike.idx] = true;
                for(size_t j=spike.idx+1;j<=n;++j)
                {
                    if( y[j] <= y[j-1])
                    {
                        used[j] = true;
                    }
                    else
                    {
                        //std::cerr << "\tused --> " << j-1 << std::endl;
                        break;
                    }
                }
                for(size_t j=spike.idx-1;j>0;--j)
                {
                    if( y[j] <= y[j+1])
                    {
                        used[j] = true;
                    }
                    else
                    {
                        //std::cerr << "\tused <-- " << j+1 << std::endl;
                        break;
                    }
                }
                
            }
        }
    }
}
