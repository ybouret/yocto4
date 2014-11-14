#include "yocto/utest/run.hpp"
#include "yocto/spade/ops/laplacian.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/sys/wtime.hpp"

using namespace yocto;
using namespace spade;

template <typename T>
static inline
void test1D()
{
    const layout1D         zone(-100,100);
    fields_setup<layout1D> F( sizeof(T) );
    Y_SPADE_FIELD_T(F, "U", array1D<T>);
    Y_SPADE_FIELD_T(F, "L", array1D<T>);
    ghosts_setup           G;
    G.set_local(on_x, 1);
    
    dataspace<layout1D>    data(zone,F,G);
    array1D<T> &U = data["U"].as< array1D<T> >();
    array1D<T> &L = data["L"].as< array1D<T> >();
    
    wtime chrono;
    chrono.start();
    const size_t ITER_MAX = 1 << 16;
    for(size_t i=0;i<ITER_MAX;++i)
    {
        laplacian<T>::compute(L,U,zone,1);
    }
    const double ell = chrono.query();
    fprintf( stderr, "speed1D(type=%5s)= %.2f M/s\n", typeid(T).name(), 1.0e-6 * double(ITER_MAX) * zone.items / (ell) );
}

template <typename T>
static inline
void test2D()
{
    const layout2D         zone( coord2D(-50,-50), coord2D(50,50) );
    fields_setup<layout2D> F( sizeof(T) );
    Y_SPADE_FIELD_T(F, "U", array2D<T>);
    Y_SPADE_FIELD_T(F, "L", array2D<T>);
    ghosts_setup           G;
    G.set_local(on_x, 1);
    G.set_local(on_y, 1);
    dataspace<layout2D>    data(zone,F,G);
    array2D<T> &U = data["U"].as< array2D<T> >();
    array2D<T> &L = data["L"].as< array2D<T> >();
    
    wtime chrono;
    chrono.start();
    const math::v2d<T> inv_dsq(1,1);
    const size_t ITER_MAX = 1 << 14;
    for(size_t i=0;i<ITER_MAX;++i)
    {
        laplacian<T>::compute(L,U,zone,inv_dsq);
    }
    const double ell = chrono.query();
    fprintf( stderr, "speed2D(type=%5s)= %.2f M/s\n", typeid(T).name(), 1.0e-6 * double(ITER_MAX) * zone.items / (ell) );
}

template <typename T>
static inline
void test3D()
{
    const layout3D         zone( coord3D(-50,-50,-50), coord3D(50,50,50 ) );
    fields_setup<layout3D> F( sizeof(T) );
    Y_SPADE_FIELD_T(F, "U", array3D<T>);
    Y_SPADE_FIELD_T(F, "L", array3D<T>);
    ghosts_setup           G;
    G.set_local(on_x, 1);
    G.set_local(on_y, 1);
    G.set_local(on_z, 1);
    
    dataspace<layout3D>    data(zone,F,G);
    array3D<T> &U = data["U"].as< array3D<T> >();
    array3D<T> &L = data["L"].as< array3D<T> >();
    
    wtime chrono;
    chrono.start();
    const math::v3d<T> inv_dsq(1,1,1);
    const size_t ITER_MAX = 1 << 8;
    for(size_t i=0;i<ITER_MAX;++i)
    {
        laplacian<T>::compute(L,U,zone,inv_dsq);
    }
    const double ell = chrono.query();
    fprintf( stderr, "speed3D(type=%5s)= %.2f M/s\n", typeid(T).name(), 1.0e-6 * double(ITER_MAX) * zone.items / (ell) );
}


YOCTO_UNIT_TEST_IMPL(ops)
{
    test1D<float>();
    test1D<double>();
    fputs("\n", stderr);
    test2D<float>();
    test2D<double>();
    fputs("\n", stderr);
    test3D<float>();
    test3D<double>();
    
}
YOCTO_UNIT_TEST_DONE()
