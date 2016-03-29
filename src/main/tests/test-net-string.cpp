#include "yocto/ios/net-string.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(net_string)
{
    ios::icstream ifp( ios::cstdin );
    ios::ocstream ofp( ios::cstderr );

    string  line;
    while( line.clear(), ifp.gets(line) )
    {
        std::cerr << "'" << line << "'" << std::endl;
        ios::net_string::write(line,ofp); ofp << "\n";
        ios::net_string::format(line,ofp); ofp << "\n";
    }

}
YOCTO_UNIT_TEST_DONE()

