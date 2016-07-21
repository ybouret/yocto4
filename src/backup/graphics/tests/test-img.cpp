#include "yocto/graphics/image/png.hpp"
#include "yocto/graphics/image/jpeg.hpp"
#include "yocto/graphics/image/tiff.hpp"
#include "yocto/graphics/rawpix.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphics;

namespace
{
    
    static void show_format( const image::format &fmt )
    {
        std::cerr << "<" << fmt.name << ">" << std::endl;
        const char **exts = fmt.extensions();
        for(size_t i=0;;++i)
        {
            const char *ext = exts[i];
            if(!ext) break;
            std::cerr << "\tmay load " << ext << std::endl;
        }
    }
    
}

#include "yocto/graphics/parallel.hpp"

YOCTO_UNIT_TEST_IMPL(img)
{
    image &IMG = image::instance();
    
    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );
    
    show_format( IMG["PNG"]  );
    show_format( IMG["JPEG"] );
    show_format( IMG["TIFF"] );
    
    const image::format &PNG = IMG["PNG"];
    //const image::format &JPG = IMG["JPEG"];
    //const image::format &TIF = IMG["TIFF"];
    const imageIO       &gfx = IMG;
    
    if(argc>1)
    {
        const string filename = argv[1];
        // load
        std::cerr << "Loading RGBA" << std::endl;
        pixmap4 p4( IMG.load4(filename, NULL));
        p4.save("image4.bmp",true);
        p4.save("image4.ppm",true);
        p4.save("image4.eps",true);
        p4.save("image4_gs.bmp",false);
        p4.save("image4_gs.ppm",false);
        p4.save("image4_gs.eps",false);

        std::cerr << "Loading RGB" << std::endl;
        pixmap3 p3( IMG.load3(filename, NULL));
        
        std::cerr << "Loading BYTE" << std::endl;
        pixmap1 p1( IMG.load1(filename, NULL));
        
        std::cerr << "Loading FLOAT" << std::endl;
        pixmapf pf( IMG.loadf(filename, NULL));

        PNG.save("image4.png",p4,NULL);
        PNG.save("image3.png",p3,NULL);
        PNG.save("image1.png",p1,NULL);
        PNG.save("imagef.png",pf,NULL);

        gfx.save("toto.png",p4,NULL);
        gfx.save("toto.jpg",p4,NULL);
        gfx.save("toto.tiff",p4,NULL);


        vector<patch> subs;
        prepare_patches(subs, 4, p4, true);
        

    }
    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(tiff2png)
{
    image &IMG = image::instance();

    tiff_format *tif = new tiff_format();
    IMG.declare( tif );
    IMG.declare( new png_format() );

    const image::format &png = IMG["PNG"];

    if(argc>1)
    {
        const string   filename = argv[1];
        const size_t   nd       = tif->count_directories(filename);
        std::cerr << "Extracting " << nd << " images" << std::endl;
        for(size_t i=0;i<nd;++i)
        {
            pixmapf pxm(tif->loadf(filename,&i));
            png.save(vformat("toto%08x.png",unsigned(i)), pxm, NULL);
        }
    }
}
YOCTO_UNIT_TEST_DONE()

