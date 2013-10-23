#include "yocto/code/entropy.hpp"
#include <cstring>
#include <cmath>

namespace yocto
{
    entropy:: ~entropy() throw()
    {
        
    }
    
    void entropy:: reset() throw()
    {
        memset(count,0,sizeof(count));
    }
    
    entropy:: entropy() throw() :
    count()
    {
        
    }
    
    double entropy:: operator()(void) const throw()
    {
        double   ans = 0;
        uint32_t sum = 0;
        for(size_t i=0;i<256;++i) sum += count[i];
        if(sum>0)
        {
            const double fac = 1.0 / sum;
        
            for(size_t i=0;i<256;++i)
            {
                const uint32_t n = count[i];
                if(n>0)
                {
                    const double p = double(n) * fac;
                    ans -= p * log(p);
                }
            }
        }
        
        return ans;
    }
    
    void entropy:: add( uint8_t b ) throw()
    {
        ++count[b];
    }
    
    void entropy:: add( const void *buffer, size_t buflen ) throw()
    {
        assert( !(buffer==0 && buflen>0) );
        const uint8_t *q = (const uint8_t *)buffer;
        while(buflen-- > 0)
        {
            ++count[*(q++)];
        }
    }

    void entropy:: add( const memory::ro_buffer &buf ) throw()
    {
        add(buf.ro(),buf.length());
    }

    
}

