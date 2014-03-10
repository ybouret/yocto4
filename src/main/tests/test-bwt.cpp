#include "yocto/utest/run.hpp"
#include "yocto/pack/bwt.hpp"
#include "yocto/ios/file-loader.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/pack/mtf.hpp"

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
        vector<char>   decode(size,0);
        
        const size_t pidx = pack::bwt::encode(&output[1], &content[1], size, &indices[1]);
        std::cerr << "pidx=" << pidx << std::endl;
        
        pack::bwt::decode(&decode[1], &output[1], size, &indices[1], pidx);
        
        for(size_t j=1;j<=size;++j)
        {
            if(decode[j]!=content[j])
                throw exception("BWT failure");
        }
        
        size_t done = 0;
        
        {
            ios::ocstream fp("org.dat",false);
            fp.put_all(&content[1], size, done);
        }
        
        {
            ios::ocstream fp("bwt.dat",false);
            fp.put_all(&output[1], size, done);
        }
        
        pack::move_to_front mtf;
        for(size_t i=1;i<=size;++i)
        {
            output[i] = mtf.encode(output[i]);
        }
        
        {
            ios::ocstream fp("mtf.dat",false);
            fp.put_all(&output[1], size, done);
        }
        
    }
}
YOCTO_UNIT_TEST_DONE()

namespace
{
    static inline
    void testBWT(array<uint8_t> &input,
                 array<uint8_t> &output,
                 array<uint8_t> &input2,
                 pack::bwt::common16 &BWT)
    {
        const uint16_t pidx = BWT.encode( &output[1], &input[1], input.size() );
        BWT.decode(&input2[1], &output[1], input.size(), pidx);
        for(size_t i=1;i<=input.size();++i)
        {
            if( input2[i] != input[i] )
                throw exception("encode/decode error for size=%u", unsigned(input.size()));
        }
    }
    
}

YOCTO_UNIT_TEST_IMPL(bwt16)
{
    pack::bwt::common16 BWT;
    vector<uint8_t> input(65536,as_capacity);
    vector<uint8_t> output(65536,as_capacity);
    vector<uint8_t> input2(65536,as_capacity);

    for(size_t n=0;n<=65536;n+=16)
    {
        if(n<=0)
            continue;
        
        input.make(n, 0);
        output.make(n,0);
        input2.make(n,0);
        
        testBWT(input, output, input2, BWT);
        
        std::cerr << "n=" << n << std::endl;
        std::cerr.flush();
        
    }
    std::cerr << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
