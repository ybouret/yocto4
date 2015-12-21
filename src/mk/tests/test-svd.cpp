#include "yocto/utest/run.hpp"
#include "yocto/math/core/svd.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(svd)
{
    for( size_t iter=1; iter <= 100; ++iter)
    {
        const size_t M = 1 + alea_lt(10);
        const size_t N = 1 + alea_lt(10);
        matrix<double> A(M,N);
        matrix<double> U(M,N);
        matrix<double> V(N,N);
        vector<double> W(N,0);
        
        for( size_t i=1; i <= M; ++i )
        {
            for(size_t j=1; j<=N;++j)
            {
                A[i][j] = (0.5-alea<double>());
            }
        }
        
        U.assign(A);
        if( svd<double>::build(U, W, V) )
        {
            std::cerr << "A=" << A << std::endl;
            std::cerr << "W=" << W << std::endl;
            svd<double>::truncate(W);
            std::cerr << "W0=" << W << std::endl;
            std::cerr << "U="  << U << std::endl;
            std::cerr << "V="  << V << std::endl;
        }
        else
        {
            std::cerr << "Unable to SVD !" << std::endl;
        }
    }
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(svdgs)
{
    
    //-- orthogonalize colums ?
    const size_t   n = 4;
    matrix<double> M(n,n);
    matrix<double> U(n,n);
    matrix<double> V(n,n);
    vector<double> W(n,0);
    
    M[1][1] = 1;
    M[2][1] = 1;
    
    M[3][2] = 1;
    M[4][2] = 1;
    
    std::cerr << "M=" << M << std::endl;
    
    U.assign(M);
    if( ! svd<double>::build(U,W,V) )
    {
        std::cerr << "Unable to svd" << std::endl;
    }
    else
    {
        std::cerr << "U=" << U << std::endl;
        std::cerr << "W=" << W << std::endl;
    }
    
    
    }
    YOCTO_UNIT_TEST_DONE()
    
    static inline void make_ortho( matrix<double> &P, matrix<double> &Q)
    {
        std::cerr << std::endl << "// Make Ortho" << std::endl;
        
        for(size_t i=1;i<=P.rows;++i)
        {
            for(size_t j=1;j<=P.cols;++j)
            {
                P[i][j] = 0.5 - alea<double>();
            }
        }
        std::cerr << "P=" << P << std::endl;
        svd<double>::orthonormal(Q, P);
        std::cerr << "Q=" << Q << std::endl;
    }
    
    YOCTO_UNIT_TEST_IMPL(ortho)
    {
        matrix<double> P;
        matrix<double> Q;
        P.make(5,2);
        
        make_ortho(P, Q);
        
        P.make(2,5);
        make_ortho(P, Q);
        
    }
    YOCTO_UNIT_TEST_DONE()
