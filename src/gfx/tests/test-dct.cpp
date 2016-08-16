#include "yocto/gfx/ops/dct.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;
YOCTO_UNIT_TEST_IMPL(dct)
{

    DCT<8> dct8;
    std::cerr << "dct8=" << dct8 << std::endl;

    for(size_t i=0;i<dct8.BLOCK_SIZE;++i)
    {
        for(size_t j=0;j<dct8.BLOCK_SIZE;++j)
        {
            for(size_t k=0;k<4;++k)
            {
                dct8.pix[k][i][j] = k+float(i+1) + float(2*j+1);
                dct8.dct[k][i][j] = -1;
            }
        }
    }

    std::cerr << "pix:" << std::endl;
    dct8.show_pix(4);
    dct8.forward(4);
    std::cerr << "dct: " << std::endl;
    dct8.show_dct(4);

}
YOCTO_UNIT_TEST_DONE()
