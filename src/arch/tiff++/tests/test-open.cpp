#include "yocto/tiff++/interface.hpp"
#include "yocto/rtld/plugin.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(open)
{

        plugin<graphics::I_TIFF> I("y-tiff");
        
        if(argc>2)
        {
            
            for(int i=2;i<argc;++i)
            {
                const string fn = argv[i];
                void *tiff = I->Open(fn.c_str(),"r");
                if(!tiff)
                {
                    throw exception("couldn't open %s",fn.c_str());
                }
                int count = 0;
                do {
                    ++count;
                    uint32_t w = 0;
                    uint32_t h = 0;
                    I->GetWidth(tiff,&w);
                    I->GetHeight(tiff,&h);
                    std::cerr << w << "x" << h << std::endl;
                    const size_t npx = w*h;
                    uint32_t *raster = new uint32_t[npx];
                    I->ReadRGBAImage(tiff,w,h,raster);
                    delete []raster;
                }
                while( I->ReadDirectory(tiff) );
                std::cerr << "Found " << count << " entry(ies)" << std::endl;
                I->Close(tiff);
            }
            
    }
    
}
YOCTO_UNIT_TEST_DONE()
