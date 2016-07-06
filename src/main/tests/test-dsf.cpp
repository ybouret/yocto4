#include "yocto/pack/dsf.hpp"
#include "yocto/pack/mtf.hpp"

#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(dsf)
{
    DSF::Encoder Q;
    move_to_front MTF;

    ios::icstream fp( ios::cstdin  );
    ios::ocstream op( ios::cstdout );
    char C = 0;
    size_t inp = 0, out=0;

    while(fp.query(C))
    {
        ++inp;
        if(0==(inp%(16*1024)))
        {
            std::cerr << ".";
            std::cerr.flush();
        }
        Q.write( MTF.encode(C) );
        while( Q.query(C) )
        {
            op.write(C);
            ++out;
        }
    }

    Q.flush();
    while( Q.query(C) )
    {
        op.write(C);
        ++out;
    }
    std::cerr << "inp=" << inp << ", out=" << out << std::endl;

}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(undsf)
{
    DSF::Decoder  Q;
    move_to_front MTF;

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
            op.write( MTF.decode(C) );
            ++out;
        }
    }

    Q.flush();
    while( Q.query(C) )
    {
        op.write( MTF.decode(C) );
        ++out;
    }

}
YOCTO_UNIT_TEST_DONE()
