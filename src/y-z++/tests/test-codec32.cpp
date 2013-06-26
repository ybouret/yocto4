#include "yocto/utest/run.hpp"
#include "yocto/z++/codec32.hpp"
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
        zlib::codec32 z32;
        
        uint32_t nOutMax = z32.EncodeSpace(content.size());
        std::cerr << "Encode Space= " << nOutMax << std::endl;
        
        memory::buffer_of<char,memory::global> encbuf( nOutMax );
        
        int32_t ans = z32.Encode(encbuf(), nOutMax, content(), content.size(), 9);
        if( ans != 0 )
        {
            std::cerr << "Couldn't Encode" << std::endl;
        }
        else
        {
            std::cerr << "Encoded Size= " << nOutMax << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

