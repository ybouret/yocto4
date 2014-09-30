#include "yocto/utest/run.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/opt/bracket.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;

template <typename T>
static inline T F( T x )
{
    return -Cos(x+T(0.23)) + T(7);
}

template <typename T>
static inline void optim()
{
    typename numeric<T>::function func( cfunctor(F<float>) );
    triplet<T> x = { -1, 0 , 1};
    triplet<T> f = { func(x.a), 0, func(x.c) };
    if( ! bracket<T>::inside( func, x, f ) )
    {
        throw exception("Couldn't bracket !");
    }
    std::cerr << "x=" << x << std::endl;
    std::cerr << "f=" << f << std::endl;
    
    minimize<T>( func,x,f,1e-8);
    std::cerr << "min@" << x.b << " = " << f.b << std::endl;
}

YOCTO_UNIT_TEST_IMPL(opt)
{
    std::cerr << "Float: " << std::endl;
    optim<float>();
    
    std::cerr << "Double: " << std::endl;
    //optim<double>();
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(opt2)
{
    numeric<double>::function func( cfunctor(F<double>) );
    triplet<double> x = { -1, 0 , 1};
    triplet<double> f = { func(x.a), 0, func(x.c) };
    if( ! bracket<double>::inside( func, x, f ) )
    {
        throw exception("Couldn't bracket !");
    }
    std::cerr << "x=" << x << std::endl;
    std::cerr << "f=" << f << std::endl;
    
    minimize2<double>( func,x,f,1e-6f);
    std::cerr << "min@" << x.b << " = " << f.b << std::endl;
    
    
    }
    YOCTO_UNIT_TEST_DONE()
