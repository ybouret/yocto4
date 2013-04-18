
#include "yocto/math/sig/spike-finder.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"

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
                std::cerr << "Looking for spike..." << std::endl;
                for( size_t i=1; i <= n; ++i )
                {
                    if( !used[i] )
                    {
                        //-- initialize
                        std::cerr << "\tFirst unused@" << i  << std::endl;
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
                        std::cerr << "Found@" << spike.idx << std::endl;
                        return true;
                    }
                }
                //-- no usable data
                std::cerr << "No more available data..." << std::endl;
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
                if( spike.pos > 1 && spike.pos < n )
                {
                    
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
                        std::cerr << "\tused --> " << j << std::endl;
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
                        std::cerr << "\tused <-- " << j << std::endl;
                        break;
                    }
                }
                
            }
        }
    }
}
