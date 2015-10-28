#include "yocto/math/point3d.hpp"
#include "yocto/utest/run.hpp"
#include "support.hpp"

template <typename T>
void fill(T* arr, const size_t num)
{
    for(size_t i=0;i<num;++i)
    {
        arr[i] = gen<T>::get();
    }
}
template <typename POINT>
static inline void doPoints()
{
    std::cerr << "doPoints" << std::endl;

    typedef typename     POINT::type type;
    static  const size_t DIM = sizeof(POINT)/sizeof(type);

    POINT p; fill(&p.x,DIM);
    std::cerr << p << std::endl;
    POINT q = p;
    std::cerr << q << std::endl;
    q = p;
    std::cerr << q << std::endl;
    q = -p;
    std::cerr << q << std::endl;
    q -= p;
    std::cerr << q << std::endl;
    q += p;
    std::cerr << q << std::endl;
    POINT r = q-p;
    std::cerr << r << std::endl;
    r = q+p;
    std::cerr << r << std::endl;

    fill(&p.x,DIM);
    std::cerr << p << std::endl;
    q = gen<type>::get() * p;
    std::cerr << q << std::endl;
    q *= gen<type>::get();
    std::cerr << q << std::endl;
    std::cerr << p*q << std::endl;
}

YOCTO_UNIT_TEST_IMPL(points)
{
    doPoints< point2d<int>   >();
    doPoints< point2d<float> >();

    doPoints< point3d<int64_t>   >();
    doPoints< point3d<double>    >();

}
YOCTO_UNIT_TEST_DONE()
