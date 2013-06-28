#include "yocto/utest/run.hpp"
#include "yocto/ios/file-loader.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstdlib>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(codec32)
{
    
    if(argc>1)
    {
        ios::file_content content;
        ios::file_loader::append_to(content, argv[1]);
        std::cerr << "Input Size  = " << content.size() << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()

