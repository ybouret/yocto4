#include "yocto/graphix/image/png.hpp"
#include "yocto/graphix/image/jpeg.hpp"
#include "yocto/graphix/image/tiff.hpp"
#include "yocto/graphix/ops/hist.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphix;

YOCTO_UNIT_TEST_IMPL(ops)
{
    image &IMG = image::instance();

    IMG.declare( new png_format()  );
    IMG.declare( new jpeg_format() );
    IMG.declare( new tiff_format() );

    threading::engine server(true);

    if(argc>1)
    {
        const string filename = argv[1];
        pixmap4      pxm( IMG.load4(filename, NULL));

        histogram H;
        H.update(pxm);
        H.save("hist.dat");

        H.reset();
        histogram::patches hp;
        histogram::create(hp,pxm,NULL);
        histogram::launch(hp,pxm,NULL);
        H.finish(hp,NULL);
        H.save("hist2.dat");

        H.reset();
        histogram::create(hp,pxm,&server);
        std::cerr << "#patches=" << hp.size() << std::endl;
        histogram::launch(hp,pxm,&server);
        H.finish(hp,&server);
        H.save("hist3.dat");

    }
}
YOCTO_UNIT_TEST_DONE()
