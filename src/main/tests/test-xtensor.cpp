#include "yocto/container/xtensor.hpp"
#include "yocto/utest/run.hpp"

#include <cstring>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(xtensor)
{

    xtensor X(512,1024,1024);
    std::cerr << "rows_size=" << X.rows_size << std::endl;
    std::cerr << "data_size=" << X.data_size << std::endl;
    std::cerr << "allocated=" << X.allocated() << std::endl;
    X.ldz();

    for(size_t j=0;j<X.rows;++j)
    {
        for(size_t i=0;i<X.cols;++i)
        {
            memset( X[j][i], i+j, X.block_size );
        }
    }

}
YOCTO_UNIT_TEST_DONE()
