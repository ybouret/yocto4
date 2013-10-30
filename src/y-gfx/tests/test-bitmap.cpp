#include "yocto/utest/run.hpp"
#include "yocto/gfx/bitmap.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(bitmap)
{
    std::cerr << "sizeof(bitmap)=" << sizeof(gfx::bitmap) << std::endl;
    gfx::bitmap::pointer bmp( gfx::bitmap::create(4,200,100) );
    gfx::bitmap::pointer sub(  gfx::bitmap::attach( *bmp, NULL)   );
    
    const gfx::region    reg2(10,10,50,50);
    gfx::bitmap::pointer sub2( gfx::bitmap::attach( *sub, &reg2)  );
    
    gfx::bitmap::pointer sub3( gfx::bitmap::carbon( *sub2,NULL) );
    
}
YOCTO_UNIT_TEST_DONE()
