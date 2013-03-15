#include "yocto/utest/run.hpp"
#include "yocto/spade/ops/laplacian.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/wtime.hpp"

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
    const size_t ITER_MAX = 10000;
    for(size_t i=0;i<ITER_MAX;++i)
    {
        laplacian<T>::compute(L,U,zone,1);
    }
    const double ell = chrono.query();
    std::cerr << "speed= " << 1.0e-6 * double(ITER_MAX) * zone.items / (ell) << " M/s" << std::endl;
}


YOCTO_UNIT_TEST_IMPL(ops)
{
    test1D<float>();
    
}
YOCTO_UNIT_TEST_DONE()
