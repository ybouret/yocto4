#include "yocto/R/R++.hpp"

using namespace yocto;

extern "C"
SEXP lyre_version() throw()
{
    Rprintf("1\n");
    return R_NilValue;
}

static inline
void Rdisp( const RArray<double> &a )
{
    Rprintf("[");
    for(size_t i=0;i<a.size();++i)
    {
        Rprintf(" %g", a[i]);
    }
    Rprintf("]\n");

}
extern "C"
SEXP lyre_sum(SEXP RA)
{
    YOCTO_R_PROLOG()
    {
        RVector<double> A(RA);
        Rdisp(A);
        RQSort(A);
        Rdisp(A);
        double sum = 0;
        for(size_t i=0;i<A.size();++i)
        {
            sum += A[i];
        }


        RVector<double> ans(1);

        ans[0] = sum;

        return *ans;
    }
    YOCTO_R_EPILOG();
}

extern "C"
SEXP lyre_mat(SEXP RM)
{
    RMatrix<double>  M(RM);
    CMatrixR<double> C(RM);
    C.print();
    return R_NilValue;
}