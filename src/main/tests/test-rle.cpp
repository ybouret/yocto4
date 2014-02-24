#include "yocto/utest/run.hpp"
#include "yocto/pack/rle.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace pack;


YOCTO_UNIT_TEST_IMPL(rle)
{
    rle::encoder RLE;
    char C = 0;
    ios::ocstream out( ios::cstdout );
    ios::icstream fp( ios::cstdin );
    while( fp.query(C) )
    {
        RLE.write(C);
        while( RLE.query(C) )
        {
            out.write(C);
        }
    }
    RLE.flush();
    while( RLE.query(C) )
    {
        out.write(C);
    }
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(unrle)
{
    rle::decoder UNRLE;
    char C = 0;
    ios::ocstream out( ios::cstdout );
    ios::icstream fp( ios::cstdin );
    while( fp.query(C) )
    {
        UNRLE.write(C);
        while( UNRLE.query(C) )
        {
            out.write(C);
        }
    }
    UNRLE.flush();
    while( UNRLE.query(C) )
    {
        out.write(C);
    }
}
YOCTO_UNIT_TEST_DONE()
