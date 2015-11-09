#include "yocto/R++/com.hpp"
#include "yocto/sort/quick.hpp"

using namespace yocto;

YOCTO_R_STATIC
const char *get_version() throw()
{
    return __DATE__;
}

YOCTO_R_FUNCTION(the_version,())
{
    Rprintf("%s\n", get_version());
    return R_NilValue;
}
YOCTO_R_RETURN()


YOCTO_R_FUNCTION(VectorSum,(SEXP vecR))
{
    RVector<double> vec( vecR );
    Rprintf("%s: #data=%u\n", __fn, unsigned(vec.size()) );
    //std::cerr << vec << std::endl;
    quicksort(vec);
    //std::cerr << vec << std::endl;
    double ans = 0;
    for(size_t i=1;i<=vec.size();++i)
    {
        ans += vec[i];
    }
    Rprintf("%s: result is %g\n", __fn, ans);

    RVector<double> s(1);
    s[1] = ans;
    return *s;

}
YOCTO_R_RETURN()


YOCTO_R_FUNCTION(MatrixTransNeg,(SEXP matR))
{
    RMatrix<double> mat(matR);
    //std::cerr << "mat=" << mat << std::endl;
    RMatrix<double> ans(mat.cols,mat.rows);
    for(size_t j=mat.cols;j>0;--j)
    {
        for(size_t i=mat.rows;i>0;--i)
        {
            ans[i][j] = -mat[j][i];
        }
    }
    return *ans;
}
YOCTO_R_RETURN()
