#include "yocto/utest/run.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

using namespace yocto;
using namespace math;

static
inline double func( double x )
{
    return 1.3+Cos(x+0.2);
}

YOCTO_UNIT_TEST_IMPL(bracket)
{
    numeric<double>::function F = cfunctor(func);

    {
        triplet<double> x = { 2, 0, 4};
        triplet<double> f = { F(x.a), 0, F(x.c) };
        std::cerr << "inside " << x << std::endl;
        bracket<double>::inside(F, x, f);
        std::cerr << "x=" << x << ", f=" << f << std::endl;
        minimize<double>(F,x,f,1e-5);
        std::cerr << "min: F(" << x.b << ")=" << f.b << std::endl;
    }

    {
        triplet<double> x = { 2, 0, 1};
        triplet<double> f = { F(x.a), 0, F(x.c) };
        std::cerr << "inside : x=" << x << ", f=" << f << std::endl;
        bracket<double>::inside(F, x, f);
        std::cerr << "x=" << x << ", f=" << f << std::endl;
        minimize<double>(F,x,f,1e-5);
        std::cerr << "min: F(" << x.b << ")=" << f.b << std::endl;
    }

    {
        triplet<double> x = { 3, 0, 4};
        triplet<double> f = { F(x.a), 0, F(x.c) };
        std::cerr << "inside : x=" << x << ", f=" << f << std::endl;
        bracket<double>::inside(F, x, f);
        std::cerr << "x=" << x << ", f=" << f << std::endl;
        minimize<double>(F,x,f,1e-5);
        std::cerr << "min: F(" << x.b << ")=" << f.b << std::endl;
    }


    std::cerr << std::endl << "with expand..." << std::endl;
    {
        triplet<double> x = { 0, 0.1 };
        triplet<double> f = { F(x.a), F(x.b) };
        
        bracket<double>::expand(F, x, f) ;
        minimize<double>(F,x,f,1e-5);
        std::cerr << "min: F(" << x.b << ")=" << f.b << std::endl;
    }
    
    {
        triplet<double> x = { 4, 5 };
        triplet<double> f = { F(x.a), F(x.b) };
        
        bracket<double>::expand(F, x, f) ;
        minimize<double>(F,x,f,1e-5);
        std::cerr << "min: F(" << x.b << ")=" << f.b << std::endl;
        
    }
}
YOCTO_UNIT_TEST_DONE()

