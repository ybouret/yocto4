#include "yocto/utest/run.hpp"
#include "yocto/json/parser.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-ptr.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(parser)
{
    JSON::Parser           json;
    auto_ptr<ios::istream> input;
    if(argc>1)
    {
        input.reset( new ios::icstream( argv[1] ) );
    }
    else
    {
        input.reset( new ios::icstream(ios::cstdin) );
    }
    
    JSON::Value  &j = json( *input );
    std::cerr << "Got " << j.type_name() << std::endl;
    std::cerr << "length=" << j.length() << std::endl;
    ios::ocstream fp( ios::cstderr );
    j.output(fp);
}
YOCTO_UNIT_TEST_DONE()
