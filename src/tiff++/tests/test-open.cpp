#include "yocto/tiff++/plugin.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(open)
{
    if(argc>1)
    {
        const string path = argv[1];
        const module tiff_dll( dylib_load(path.c_str(), dylib_full) );
        gfx::TIFF_Interface I(tiff_dll,"load_tiff");
        
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
                }
                while( I->ReadDirectory(tiff) );
                std::cerr << "Found " << count << " entry(ies)" << std::endl;
                I->Close(tiff);
            }
            
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
