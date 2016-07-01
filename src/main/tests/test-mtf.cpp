#include "yocto/pack/mtf.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(mtf)
{
    pack::move_to_front MTF;

    ios::icstream src( ios::cstdin  );
    ios::ocstream dst( ios::cstdout );

    char C = 0;

    while( src.query(C) )
    {
        dst.write( MTF.encode(C) );
    }

}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(unmtf)
{
    pack::move_to_front MTF;

    ios::icstream src( ios::cstdin  );
    ios::ocstream dst( ios::cstdout );

    char C = 0;

    while( src.query(C) )
    {
        dst.write( MTF.decode(C) );
    }

}
YOCTO_UNIT_TEST_DONE()