#include "yocto/R/R++.hpp"

using namespace yocto;

extern "C"
SEXP lyre_version() throw()
{
    Rprintf("1\n");
    return R_NilValue;
}

extern "C"
SEXP lyre_sum(SEXP RA)
{
    RVector<double> A(RA);
    double sum = 0;
    for(size_t i=0;i<A.size();++i)
    {
        sum += A[i];
    }
    
    RVector<double> ans(1);
    
    ans[0] = sum;
    
    return *ans;
}

extern "C"
SEXP lyre_trn(SEXP RM)
{
    RMatrix<double> M(RM);
    
    CMatrixR<double> C(RM);
    
    return R_NilValue;
}