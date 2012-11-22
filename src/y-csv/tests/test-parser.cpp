#include "yocto/utest/run.hpp"
#include "yocto/csv/parser.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(parser)
{
    CSV::Parser   parse;
    CSV::Lines    lines;
    
    {
        ios::icstream fp( ios::cstdin );
        parse( lines, fp );
    }
    for( size_t i=1; i <= lines.size(); ++i )
    {
        std::cerr << lines[i] << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
