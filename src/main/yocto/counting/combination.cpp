#include "yocto/counting/combination.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

#include "yocto/memory/global.hpp"
#include <iostream>

#include "yocto/code/binary.hpp"
#include "yocto/code/endian.hpp"

#include <cstring>

namespace yocto
{
    namespace
    {
        static size_t __bytes_for(size_t n )
        {
            if(n<=0)
                throw libc::exception( EDOM, "invalid n=%u for combination",unsigned(n));
            const size_t ans = bytes_for(n-1);

            return ans;
        }
    }

    combination:: combination( size_t an, size_t ak ) :
    n(an),
    k(ak),
    id(0),
    comb(0),
    nmk(ptrdiff_t(n)-ptrdiff_t(k)),
    nmkp1(nmk+1),
    bytes_per_index(__bytes_for(n)   ),
    bytes_per_frame(k*bytes_per_index)
    {
        if(k<=0||k>n)
            throw libc::exception( EDOM, "invalid combination(%u,%u)", unsigned(n), unsigned(k) );

        comb = static_cast<size_t *>(memory::global:: __calloc(k,sizeof(size_t)));
        assert(comb);
        init();
    }

    combination:: combination(const combination &C ) :
    n(C.n),
    k(C.k),
    id(C.id),
    comb(0),
    nmk(C.nmk),
    nmkp1(C.nmkp1),
    bytes_per_index(C.bytes_per_index),
    bytes_per_frame(C.bytes_per_frame)
    {
        comb = static_cast<size_t *>(memory::global:: __calloc(k,sizeof(size_t)));
        for(size_t i=0;i<k;++i)
        {
            comb[i] = C.comb[i];
        }
    }

    combination:: ~combination() throw()
    {
        memory::global::__free(comb);
    }

    void combination:: init() throw()
    {
        for(size_t i=0;i<k;++i)
        {
            comb[i] = i;
        }
        (uint64_t&)id = 1;
    }

    bool combination:: next() throw()
    {
        const ptrdiff_t sk = ptrdiff_t(k);
        ptrdiff_t       i  = sk - 1;
        //++comb[i];
        while( (i>=0) && (ptrdiff_t(++comb[i])>=nmkp1+i) )
        {
            --i;
            //++comb[i];
        }

        if (ptrdiff_t(comb[0])>nmk)  // Combination (n-k, n-k+1, ..., n) reached
            return false; // No more combinations can be generated

        // comb now looks like (..., x, n, n, n, ..., n).
        // Turn it into (..., x, x + 1, x + 2, ...)
        for (++i; i<sk; ++i)
        {
            comb[i] = comb[i-1] + 1;
        }
        ++ ( (uint64_t&)id );
        return true;

    }





    std::ostream & operator<<( std::ostream &os, const combination &C )
    {
        os << "[";
        for(size_t i=0;i<C.k;++i) os << ' ' << C.comb[i];
        os << " ]'";
        return os;
    }


    uint64_t combination:: count_all() throw()
    {
        init();
        while(next())
            ;
        return id;
    }

    void * combination::save(void *addr) const
    {
        if(bytes_per_frame)
        {
            assert(addr!=0);
            uint8_t *p = (uint8_t *)addr;
            for(size_t i=0;i<k;++i)
            {
                const size_t K = swap_be_as<size_t>(comb[i]);
                memcpy(p,&K,bytes_per_index);
                p += bytes_per_index;
            }
            return p;
        }
        else
        {
            return addr;
        }
    }

    
}
