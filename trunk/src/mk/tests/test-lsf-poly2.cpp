#include "yocto/utest/run.hpp"
#include "yocto/math/fit/least-squares.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/ipower.hpp"

using namespace yocto;
using namespace math;

static inline void
save_data( const char *filename, const least_squares<double>::sample &S)
{
    ios::ocstream fp(filename,false);
    for(size_t i=1;i<=S.N;++i)
    {
        fp("%g %g %g\n", S.X[i], S.Y[i], S.Z[i] );
    }
    
}

YOCTO_UNIT_TEST_IMPL(lsf_poly2)
{
    
    for(size_t p=0;p<=10;++p)
    {
        std::cerr << "0.1^" << p << " = " << ipower<double>(0.1,p) << std::endl;
    }
    
    size_t N = 70;
    vector<double> X(N,0);
    vector<double> Y(N,0);
    for(size_t i=1;i<=N;++i)
    {
        X[i] = 0.5 * numeric<double>::pi * alea<double>();
        Y[i] = Sin(X[i]) + 0.1 * ( alea<double>()-0.5);
    }
    co_qsort(X,Y);
    vector<double> Z(N,0);
    
    std::cerr << "X=" << X << std::endl;
    std::cerr << "Y=" << Y << std::endl;
    least_squares<double>::sample S(X,Y,Z);
    S.prepare(1);
    {
        vector<double> aorg(1,0);
        vector<double> aerr(1,0);
        vector<bool>   used(1,true);
        
        S.polynomial(aorg, used, aerr, NULL);
        
        save_data("p0.dat",S);
      
    }
    
    
    {
        vector<double> aorg(2,0);
        vector<double> aerr(2,0);
        vector<bool>   used(2,true);
        
        S.polynomial(aorg, used, aerr, NULL);
        
        save_data("p1.dat",S);
        
    }
    
    
    {
        vector<double> aorg(2,0);
        vector<double> aerr(2,0);
        vector<bool>   used(2,true);
        
        used[1] = false;
        S.polynomial(aorg, used, aerr, NULL);
        
        save_data("p1b.dat",S);
        
    }
    
    {
        vector<double> aorg(3,0);
        vector<double> aerr(3,0);
        vector<bool>   used(3,true);
        
        S.polynomial(aorg, used, aerr, NULL);
        
        save_data("p2.dat",S);
        
    }

    
    
}
YOCTO_UNIT_TEST_DONE()
