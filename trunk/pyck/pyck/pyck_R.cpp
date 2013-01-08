#include "./os.hpp"
#include "./R++.hpp"

extern "C"
SEXP dumpVec( SEXP args ) throw()
{
    const RVector<double> v(args);
    Rprintf("\t#items=%u\n", unsigned(v.size));
    for(size_t i=0; i < v.size;++i)
    {
        Rprintf("\tv[%2u] = %g\n",unsigned(i), v[i]);
    }
    return R_NilValue;
}

extern "C"
SEXP sumVec( SEXP args ) throw()
{
    try
    {
        const RVector<double> v(args);
        RVector<double>       ans(1);
        double sum = 0;
        for( size_t i=0; i < v.size;++i)
        {
            sum += v[i];
        }
        ans[0] = sum;
        
        return *ans;
    }
    catch(...)
    {
        Rprintf("an error occured!\n");
        return R_NilValue;
    }
}
