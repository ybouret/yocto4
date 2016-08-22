#include "yocto/gfx/ops/dct.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gfx;


YOCTO_UNIT_TEST_IMPL(dct)
{
    
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
        SquareDCT sdct(w);

        for(unit_t h=1;h<=16;++h)
        {
            std::cerr << std::endl;
            std::cerr << "w=" << w << ", h=" << h << std::endl;
            CommonDCT cdct(w,h);

            cdct.forward(ipx,xx,yy);
            cdct.pix.ldz();
            cdct.reverse();

            if(h==w)
            {
                sdct.forward(ipx,xx,yy);
                sdct.pix.ldz();
                sdct.reverse();
            }

            for(unit_t j=0;j<cdct.h;++j)
            {
                for(unit_t i=0;i<cdct.w;++i)
                {
                    std::cerr << "(" << ipx[yy+j][xx+i] << "," << cdct.pix[j][i];
                    if(h==w)
                    {
                        std::cerr << "," << sdct.pix[j][i];
                    }
                    std::cerr << ") ";
                }
                std::cerr << std::endl;
            }

        }
    }
}
YOCTO_UNIT_TEST_DONE()
