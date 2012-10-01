#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/svd.hpp"
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
            svd<double>::truncate(W, 1e-6);
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

