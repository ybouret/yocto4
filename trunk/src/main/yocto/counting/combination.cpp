#include "yocto/counting/combination.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

#include "yocto/memory/global.hpp"
#include <iostream>

namespace yocto
{
    
    
    combination:: combination( size_t an, size_t ak ) :
    n(an),
    k(ak),
    id(0),
    comb(0),
    nmk(ptrdiff_t(n)-ptrdiff_t(k)),
    nmkp1(nmk+1)
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
    nmkp1(C.nmkp1)
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
        ptrdiff_t i = ptrdiff_t(k) - 1;
        //++comb[i];
        while( (i>=0) && (++comb[i]>=nmkp1+i) )
        {
            --i;
            //++comb[i];
        }
        
        if (comb[0]>nmk)  // Combination (n-k, n-k+1, ..., n) reached
            return false; // No more combinations can be generated
        
        // comb now looks like (..., x, n, n, n, ..., n).
        // Turn it into (..., x, x + 1, x + 2, ...)
        for (++i; i<k; ++i)
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

    
}