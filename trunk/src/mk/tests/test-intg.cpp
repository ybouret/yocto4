#include "yocto/utest/run.hpp"
#include "yocto/math/fcn/integrate.hpp"

using namespace yocto;
using namespace math;

namespace  {
    
    template <typename T>
    T F( T x )
    {
        const T y = Cos(x);
        return y*y;
    }
    
    template <typename T>
    T F2(T x, T y)
    {
        return 1;
    }
    
    template <typename T>
    void Bounds( T x, T &ylo, T &yhi )
    {
        yhi = Fabs(x) >= 1 ? 0 : Sqrt( numeric<T>::one - x*x);
        ylo = -yhi;
    }
    
}


YOCTO_UNIT_TEST_IMPL(intg)
{
    std::cerr << "1D integral" << std::endl;
    {
        numeric<float>::function func( cfunctor( F<float>) );
        const float              ans  = integrate<float>( 0, numeric<float>::two_pi, func, 1e-5f )/numeric<float>::pi;
        std::cerr << "float:  " << ans << std::endl;
    }
    
    {
        numeric<double>::function func( cfunctor( F<double>) );
        const double              ans  = integrate<double>( 0, numeric<double>::two_pi, func, 1e-5 )/numeric<double>::pi;
        std::cerr << "double: " << ans << std::endl;
    }
    
    std::cerr << "2D integral" << std::endl;
    {
        numeric<double>::function2 func2(  cfunctor2( F2<double> )    );
        integration<double>::boundaries   bounds( cfunctor3( Bounds<double>) );
        const double ans = integrate<double>(-1, 1, bounds, func2, 1e-5);
        std::cerr << "circle: " << ans << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
