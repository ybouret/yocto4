#include "yocto/utest/run.hpp"
#include "yocto/pack/bwt.hpp"
#include "yocto/ios/file-loader.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bwt)
{
    if(argc<=1)
        return 0;
    
    ios::file_content content;
    ios::file_loader::append_to(content, argv[1]);
    
    const size_t   size = content.size();
    if(size>0)
    {
        vector<size_t> indices(size,0);
        vector<char>   output(size,0);
        const size_t pidx = pack::bwt::encode(&output[1], &content[1], size, &indices[1]);
        std::cerr << "pidx=" << pidx << std::endl;
        
        ios::ocstream fp("bwt.dat",false);
        size_t done = 0;
        fp.put_all(&output[1], size, done);
        
    }
}
YOCTO_UNIT_TEST_DONE()

