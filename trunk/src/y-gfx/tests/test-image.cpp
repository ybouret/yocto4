#include "yocto/utest/run.hpp"
#include "yocto/gfx/image.hpp"
#include "yocto/gfx/image/tga.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(image)
{
    gfx::image &img = gfx::image::instance();
    std::cerr << "testing " << img.get_name() << std::endl;
    
    img( new gfx::TGA() );
    std::cerr << "supported formats:" << std::endl;
    for( gfx::image::iterator i=img.begin();i!=img.end();++i)
    {
        std::cerr << (*i)->name << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
