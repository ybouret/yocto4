#include "yocto/utest/run.hpp"
#include "yocto/json/parser.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(parser)
{
    JSON::Parser  json;
    ios::icstream in( ios::cstdin );
    JSON::Value  &j = json( in );
    std::cerr << "Got " << j.type_name() << std::endl;
    std::cerr << "length=" << j.length() << std::endl;
}
YOCTO_UNIT_TEST_DONE()
