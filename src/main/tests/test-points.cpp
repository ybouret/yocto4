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


#include "yocto/parallel/splitter.hpp"

using namespace parallel;

YOCTO_UNIT_TEST_IMPL(splitting)
{
    patch1D p1(-10,10);
    std::cerr << "p1=" << p1 << std::endl;

    vector<patch1D> sub1;

    for(size_t cpus=1;cpus<=8;++cpus)
    {
        std::cerr << "\t\t#CPUS=" << cpus << std::endl;
        split1D S(cpus,p1);
        std::cerr << "\tcores=" << S.cores << std::endl;
        for(size_t rank=0;rank<S.cores;++rank)
        {
            const patch1D p = S(rank);
            std::cerr << S.cores << "." << rank << " : " << p << std::endl;
        }
        build_patches(sub1, cpus, p1);
    }



    patch2D p2( coord2D(1,1), coord2D(10,5) );
    std::cerr << "p2=" << p2 << std::endl;
    vector<patch2D> sub2;
    for(size_t cpus=1;cpus<=8;++cpus)
    {
        std::cerr << "\t\t#CPUS=" << cpus << std::endl;
        split2D S(cpus,p2);
        std::cerr << "\tcores  =" << S.cores << std::endl;
        std::cerr << "\tx_cores=" << S.x_cores << std::endl;
        std::cerr << "\ty_cores=" << S.y_cores << std::endl;
        for(size_t rank=0;rank<S.cores;++rank)
        {
            const patch2D p = S(rank);
            std::cerr << S.cores << "." << rank << " : " << p << std::endl;
        }
        build_patches(sub2, cpus, p2);
    }
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/parallel/server-split.hpp"

YOCTO_UNIT_TEST_IMPL(engine_split)
{
    threading::engine server(true);
    patch2D p2( coord2D(1,1), coord2D(10,5) );
    vector<patch2D> sub2;
    create_patches(sub2,p2,&server);
    std::cerr << "patch2=" << p2 << std::endl;
    for(size_t i=1;i<=sub2.size();++i)
    {
        std::cerr << "sub#" << i << " : " << sub2[i] << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()

