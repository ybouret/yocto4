#include "yocto/utest/run.hpp"
#include "yocto/hashing/rmd.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(rmd)
{
    
    hashing::rmd  RMD;
    ios::icstream fp( ios::cstdin );
    char C = 0;
    while( fp.query(C) )
    {
        if( RMD.store(C) )
        {
            
        }
    }
    const size_t nleft = RMD.length & 63;
    std::cerr << "nleft=" << nleft << std::endl;
    const uint8_t *b = RMD.flush();
    std::cerr << "[";
    for( size_t i=0; i < nleft; ++i)
        std::cerr << char(b[i]);
    std::cerr << "]" << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/hashing/rmd160.hpp"
#include "yocto/hashing/digest.hpp"

YOCTO_UNIT_TEST_IMPL(rmd160)
{
    hashing::rmd160 RMD160;
    
    {
        const string vec;
        const digest i_md = digest::hex("9c1185a5c5e9fc54612808977ee8f548b2258d31");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        const string vec = "a";
        const digest i_md = digest::hex("0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        const string vec = "abc";
        const digest i_md = digest::hex("8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        const string vec = "message digest";
        const digest i_md = digest::hex("5d0689ef49d2fae572b881b123a85ffa21595f36");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        const string vec = "abcdefghijklmnopqrstuvwxyz";
        const digest i_md = digest::hex("f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        const string vec = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        const digest i_md = digest::hex("12a053384a9c0c88e405a06c27dcf49ada62eb2b");
        const digest o_md = digest::checksum(RMD160,vec);
        std::cerr << "=>" << o_md << "/" << i_md << std::endl;
    }
    
    {
        RMD160.set();
        const char a = 'a';
        for( size_t i=0; i < 1000000;++i)
        {
            RMD160.run( &a, 1);
        }
        const digest i_md = digest::hex("52783243c1697bdbe16d37f97f68f08325dc1528");
        const digest o_md = digest::get_from(RMD160);
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
