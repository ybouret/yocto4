#include "yocto/code/combination.hpp"

namespace yocto
{
    
    void combination:: init( size_t comb[], const size_t k, const size_t n ) throw()
    {
        assert(comb!=0);
        assert(k<=n);
        for(size_t i=0;i<k;++i) comb[i] = i;
    }
    
    bool combination:: next( size_t comb[], const size_t k, const size_t n ) throw()
    {
        assert(comb!=0);
        assert(k<=n);
        const ptrdiff_t nmk = n-k;
        ptrdiff_t       i   = ptrdiff_t(k) - 1;
        ++comb[i];
        while ((i >= 0) && (comb[i] >= nmk + 1 + i)) {
            --i;
            ++comb[i];
        }
        
        if (comb[0] > nmk) /* Combination (n-k, n-k+1, ..., n) reached */
            return false; /* No more combinations can be generated */
        
        /* comb now looks like (..., x, n, n, n, ..., n).
         Turn it into (..., x, x + 1, x + 2, ...) */
        for (i = i + 1; i < k; ++i)
            comb[i] = comb[i - 1] + 1;
        
        return true;
    }
    
}