#include "yocto/utest/run.hpp"
#include "yocto/json/parser.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(parser)
{
    JSON::Parser           json;
#if 1
    auto_ptr<ios::istream> input(argc > 1 ? new ios::icstream( argv[1] ) : new ios::icstream(ios::cstdin));
    
    
    JSON::Value  &j = json( *input );
    std::cerr << "Got "    << j.type_name() << std::endl;
    std::cerr << "length=" << j.length() << std::endl;
    ios::ocstream fp( ios::cstderr );
    j.output(fp);
#endif
}
YOCTO_UNIT_TEST_DONE()
