#include "yocto/gfx/ops/dct.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;

template <size_t N>
static inline void run_dct()
{

    std::cerr << std::endl;
    std::cerr << "----------------" << std::endl;
    std::cerr << "DCT<" << N << ">" << std::endl;
    std::cerr << "----------------" << std::endl;

    DCT0<N> dct;
    std::cerr << "sizeof(DCT)=" << sizeof(DCT0<N>) << std::endl;

    for(size_t i=0;i<dct.BLOCK_SIZE;++i)
    {
        for(size_t j=0;j<dct.BLOCK_SIZE;++j)
        {
            for(size_t k=0;k<4;++k)
            {
                dct.pix[k][i][j] = double(k)+double(i+1) + double(2*j+1);
            }
        }
    }

    std::cerr << "pix:" << std::endl;
    dct.show_pix(1);
    dct.forward(4);
    std::cerr << "dct: " << std::endl;
    dct.show_dct(1);
    dct.zpix();
    dct.reverse(4);
    std::cerr << "pix:" << std::endl;
    dct.show_pix(1);

}

YOCTO_UNIT_TEST_IMPL(dct)
{
    run_dct<1>();
    run_dct<2>();
    run_dct<3>();
    run_dct<4>();
    run_dct<8>();
    run_dct<10>();
    run_dct<16>();

    pixmap<int> ipx(100,200);
    for(unit_t j=0;j<ipx.h;++j)
    {
        for(unit_t i=0;i<ipx.w;++i)
        {
            ipx[j][i] = (i+1) + (2*j+1);
        }
    }

    const unit_t xx = 0;
    const unit_t yy = 0;

    for(unit_t w=1;w<=16;++w)
    {
        for(unit_t h=1;h<=16;++h)
        {
            std::cerr << std::endl;
            std::cerr << "w=" << w << ", h=" << h << std::endl;
            DCT dct(w,h);

            dct.forward(ipx,xx,yy);
            dct.reverse();
            for(unit_t j=0;j<dct.h;++j)
            {
                for(unit_t i=0;i<dct.w;++i)
                {
                    std::cerr << "(" << ipx[yy+j][xx+i] << "," << dct.pix[j][i] << ") ";
                }
                std::cerr << std::endl;
            }

        }
    }
}
YOCTO_UNIT_TEST_DONE()
