#include "yocto/pack/dsf.hpp"
#include "yocto/pack/mtf.hpp"

#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/hashing/sha1.hpp"

#include "yocto/hashing/md5.hpp"

using namespace yocto;
using namespace pack;
#include "yocto/code/utils.hpp"

YOCTO_UNIT_TEST_IMPL(dsf)
{
    DSF::Encoder Q;

    ios::icstream fp( ios::cstdin  );
    ios::ocstream op( ios::cstdout );

    hashing::md5 H;
    hashing::md5 G;



    char C = 0;
    size_t inp = 0, out=0;

    H.set();
    G.set();
    while(fp.query(C))
    {
        ++inp;
        if(0==(inp%(16*1024)))
        {
            std::cerr << ".";
            std::cerr.flush();
        }
        Q.write( C );
        H.run(&C,1);

        while( Q.query(C) )
        {
            op.write(C);
            G.run(&C,1);
            ++out;
        }
    }

    Q.flush();
    while( Q.query(C) )
    {
        op.write(C);
        G.run(&C,1);
        ++out;
    }
    std::cerr << "inp=" << inp << ", out=" << out << std::endl;
    uint8_t key[ hashing::md5::__length ];
    
    std::cerr << "ikey=";
    H.get(key,sizeof(key));
    for(size_t i=0;i<sizeof(key);++i)
    {
        std::cerr << hexa_text[key[i]];
    }
    std::cerr << std::endl;

    std::cerr << "okey=";
    G.get(key,sizeof(key));
    for(size_t i=0;i<sizeof(key);++i)
    {
        std::cerr << hexa_text[key[i]];
    }
    std::cerr << std::endl;


}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(undsf)
{
    DSF::Decoder  Q;

    ios::icstream fp( ios::cstdin  );
    ios::ocstream op( ios::cstdout );
    char C = 0;
    size_t inp = 0, out=0;

    while(fp.query(C))
    {
        ++inp;
        Q.write(C);
        while( Q.query( C ) )
        {
            op.write( C );
            ++out;
        }
    }

    Q.flush();
    while( Q.query(C) )
    {
        op.write( C );
        ++out;
    }
    std::cerr << "inp=" << inp << ", out=" << out << std::endl;
}
YOCTO_UNIT_TEST_DONE()
