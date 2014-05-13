#include "yocto/code/combination.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include "yocto/memory/global.hpp"
#include <iostream>

namespace yocto
{
    
    
    combination:: combination( size_t an, size_t ak ) :
    n(an),
    k(ak),
    comb(0),
    nmk(ptrdiff_t(n)-ptrdiff_t(k)),
    nmkp1(nmk+1)
    {
        if(k>n||k<=0) throw libc::exception( EDOM, "invalid combination(%u,%u)", unsigned(k), unsigned(n) );
        comb = static_cast<size_t *>(memory::global:: __calloc(k,sizeof(size_t)));
        init();
    }
    
    combination:: ~combination() throw()
    {
        memory::global::__free(comb);
    }
    
    void combination:: init() throw()
    {
        assert(comb!=0);
        assert(k<=n);
        for(size_t i=0;i<k;++i) comb[i] = i;
    }
    
    bool combination:: next() throw()
    {
        assert(comb!=0);
        assert(k<=n);
        ptrdiff_t i = ptrdiff_t(k) - 1;
        ++comb[i];
        while ((i >= 0) && (comb[i] >= nmkp1 + i))
        {
            --i;
            ++comb[i];
        }
        
        if (comb[0]>nmk)  // Combination (n-k, n-k+1, ..., n) reached
            return false; // No more combinations can be generated
        
        // comb now looks like (..., x, n, n, n, ..., n).
        // Turn it into (..., x, x + 1, x + 2, ...)
        for (++i; i<k; ++i)
            comb[i] = comb[i-1] + 1;
        
        return true;

    }
 
    size_t combination:: operator[](size_t i) const throw()
    {
        assert(i<k);
        assert(comb);
        return comb[i];
    }

    std::ostream & operator<<( std::ostream &os, const combination &C )
    {
        os << "[";
        for(size_t i=0;i<C.k;++i) os << " " << C.comb[i];
        os << " ]'";
        return os;
    }
    
}