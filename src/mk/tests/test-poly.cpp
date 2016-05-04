#include "yocto/utest/run.hpp"
#include "yocto/math/core/polynomial.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(poly)
{
    typedef polynomial<double> Poly;
    
    Poly P;
    
    std::cerr << P << std::endl;
    P.add(2,2);
    std::cerr << P << std::endl;
    P.add(1,3);
    std::cerr << P << std::endl;
    P.add(0,1);
    std::cerr << P << std::endl;
    
    P.clear();
    
    P.add(0,2);
    P.add(1,3);
    P.add(2,4);
    std::cerr << "P=" << P << std::endl;
    
    Poly Q;
    Q.add(3,2);
    Q.add(6,8);
    
    std::cerr << "Q=" << Q << std::endl;
    
    P = Poly::mul(P, Q);
    std::cerr << "PQ=" << P << std::endl;
    std::cerr << "degree=" << P.degree() << std::endl;
    while(P.degree()>0)
    {
        P = Poly::derivative(P);
        std::cerr << "dP=" << P << std::endl;
    }
    P.clear();
    
    P.add(0,2);
    P.add(1,3);
    P.add(2,4);
    std::cerr << "P=" << P << std::endl;
    P = Poly::primitive(P);
    std::cerr << "iP=" << P << std::endl;
    
    P.clear();
    Q.clear();
    
    P.add(0,1);
    P.add(1,2);
    P.add(2,3);
    
    Q = P;
    
    std::cerr << "P=" << P << std::endl;
    std::cerr << "Q=" << Q << std::endl;
    Poly::derivative(P,Q);
    std::cerr << P << "/" << Q << std::endl;
    
    
    
}
YOCTO_UNIT_TEST_DONE()
